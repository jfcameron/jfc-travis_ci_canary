// © 2020 Joseph Cameron - All Rights Reserved
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <jfc/travis_ci_canary/icon.h>
#include <jfc/travis_ci_canary/notification.h>
#include <jfc/travis_ci_canary/delete_me.h>

using namespace jfc::travis_ci_canary;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FETCH
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// HEADER///////////////////////////////////////
using response_handler_type = std::function<void(std::vector<unsigned char>)>;

//////////// CPP STATIC ///////////////////////////////////////////
// Buffer in system memory, used to store binary data fetched from remote server
struct MemoryStruct 
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *const contentPointer, 
    const size_t contentItemSize, 
    const size_t contentItemCount, 
    void *const userPointer)
{
    const size_t contentByteCount(contentItemSize * contentItemCount);

    auto pResponseBuffer(static_cast<struct MemoryStruct *const>(userPointer));

    return [&contentPointer, &contentByteCount, &pResponseBuffer]()
    {
        if ((pResponseBuffer->memory = static_cast<char *>(realloc(pResponseBuffer->memory, 
            pResponseBuffer->size + contentByteCount + 1))) == nullptr)
            throw std::runtime_error(std::string("BLAR")
                .append("gdk::resources::remote fetch failed: could not allocate system memory to store fetched content!"));

        memcpy(&(pResponseBuffer->memory[pResponseBuffer->size]), contentPointer, contentByteCount);

        pResponseBuffer->size += contentByteCount;

        pResponseBuffer->memory[pResponseBuffer->size] = {0};

        return contentByteCount;
    }();
    
    return 0;
}

///////////////// REFACTOR //////////////////////////////
// 1: REPLACE SIMPLE WITH ASYNC CURL
// 2: Good opportunity to object orientate. http::request_get, post, ...
// would be separate lib, oo abstraction on curl. middle ground would be
// write specialized get (travis_get) here, then use that as inspiration for
// an http request lib.
// an object would be useful even in the program, in case i need to expand to
// support multiple requests.
/////////////////////////////////////////////////////////
void do_request(std::string aTravisToken, response_handler_type aHandler)
{
    std::string aURL("https://api.travis-ci.org/builds?limit=50&sort_by=started_at:desc");

    curl_global_init(CURL_GLOBAL_ALL);

    if (CURL * curl_handle = curl_easy_init())
    {
        struct MemoryStruct chunk = (MemoryStruct)
        {
            .memory = []()
            {
                if (auto pHeap = static_cast<char *>(malloc(1))) return pHeap;
                else throw std::runtime_error(std::string("BLAR")
                    .append("could not allocate space on the heap"));
            }(),
            .size = 0
        };

        // Configure the request
        curl_easy_setopt(curl_handle, CURLOPT_URL, aURL.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_slist *headerlist = curl_slist_append(NULL, "Travis-API-Version: 3");
        headerlist = curl_slist_append(headerlist, 
            std::string(std::string("Authorization: token ") + aTravisToken).c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // send all data to this function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk); // we pass our 'chunk' struct to the callback function

        // perform request
        const CURLcode curlResult = curl_easy_perform(curl_handle);

        if (curlResult == CURLE_OK)
            aHandler(std::vector<unsigned char>(chunk.memory, chunk.memory + chunk.size));

        else throw std::runtime_error(std::string("BLAR")
            .append("curl_easy_perform failed: ")
            .append(curl_easy_strerror(curlResult)));

        curl_easy_cleanup(curl_handle);

        free(chunk.memory);
    }
    else throw std::runtime_error(std::string("BLAR").append("Failed to initialize a curl session."));

    curl_global_cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       MAIN.CPP
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void response_handler(std::vector<unsigned char> output)
{
    //std::vector<unsigned char> output(chunk.memory, chunk.memory + chunk.size); //arg?

    struct build_info
    {
        state_type state;
        
        std::time_t time;
    };

    std::unordered_map</*repo name*/std::string, build_info> build_info_set;

    using namespace nlohmann;

    const json root = json::parse(output);

    const auto builds = root["builds"];

    for (const auto &build : builds)
    {
        const auto state = build["state"].is_string()
            ? std::string(build["state"]) 
            : "failed";

        const auto committed_at = build["commit"]["committed_at"].is_string()
            ? std::string(build["commit"]["committed_at"])
            : "unknown";

        const auto commit_sha = build["commit"]["sha"].is_string()
            ? std::string(build["commit"]["sha"])
            : "unknown";

        const auto repo_name = build["repository"]["name"].is_string()
            ? std::string(build["repository"]["name"])
            : "unknown";


        //TODO: assert 8601 format in string

        // Converting ISO8601 string to std::tm to std::time_t (unix time)
        int y,M,d,h,m;
        float s;

        sscanf(std::string(committed_at).c_str(), 
            "%d-%d-%dT%d:%d:%fZ", 
            &y, &M, &d, &h, &m, &s);

        std::tm time;

        time.tm_year = y - 1900; // Year since 1900
        time.tm_mon = M - 1;     // 0-11
        time.tm_mday = d;        // 1-31
        time.tm_hour = h;        // 0-23
        time.tm_min = m;         // 0-59
        time.tm_sec = static_cast<int>(s); // 0-60

        std::time_t unix_time = std::mktime(&time);

        // Insert this data IF it is more recent than what is already there
        auto search = build_info_set.find(std::string(repo_name)); 

        if (search == build_info_set.end())
        {
            build_info newinfo = {
                [](std::string aState)
                {
                    if (aState == "passed") return state_type::succeeded;
                    if (aState == "failed") return state_type::failed;
                    if (aState == "errored") return state_type::failed;
                    if (aState == "cancelled") return state_type::cancelled;
                    if (aState == "canceled") return state_type::cancelled;
                    if (aState == "created") return state_type::building;
                    if (aState == "started") return state_type::building;

                    throw std::invalid_argument(std::string("unhandled state type: ").append(aState));
                }(std::string(state)),

                unix_time
            };

            build_info_set[std::string(repo_name)] = newinfo;
        }
    }

    state_type state = state_type::succeeded;

    for (const auto &[key, value] : build_info_set)
    {
        if (value.state == state_type::building) 
        {
            state = state_type::building;
        }
        else if (value.state == state_type::failed) 
        {
            state = state_type::failed;

            break;
        }
    }

    if (state == state_type::succeeded) jfc::travis_ci_canary::icon::set_icon_tooltip("all builds succeeded");
    else if (state == state_type::building) jfc::travis_ci_canary::icon::set_icon_tooltip("building");
    else if (state == state_type::failed) jfc::travis_ci_canary::icon::set_icon_tooltip("a build has failed");

    jfc::travis_ci_canary::icon::set_tray_icon(state);
}

bool update(std::string *aTravisToken)
{
    std::cout << "updated\n";

    do_request(*aTravisToken, &response_handler);

    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2) throw std::invalid_argument("need 1 arg: the travis token");

    std::string travisToken = argv[1];

    gtk_init(&argc, &argv);

    update(&travisToken);

    g_timeout_add_seconds(15, reinterpret_cast<GSourceFunc>(update), &travisToken);
   
    gtk_main();

    return EXIT_SUCCESS;
}

