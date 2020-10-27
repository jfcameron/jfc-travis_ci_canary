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

#include <jfc/icons.h>

///// move to separate file, state that can be reused (ie the request) move to class members, or at least a static

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

void do_request(std::string aTravisToken)
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
        curl_easy_setopt(curl_handle, CURLOPT_URL, aURL.c_str()); // specify URL to get
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // some servers require an agent

        curl_slist *headerlist = curl_slist_append(NULL, "Travis-API-Version: 3");
        headerlist = curl_slist_append(headerlist, 
            std::string(std::string("Authorization: token ") + aTravisToken).c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // send all data to this function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk); // we pass our 'chunk' struct to the callback function

        // perform request
        const CURLcode curlResult = curl_easy_perform(curl_handle);

        std::cout << "performing request\n";

        if (curlResult == CURLE_OK)
        {
            struct build_info
            {
                bool passed;

                std::time_t time;
            };

            std::unordered_map</*repo name*/std::string, build_info> build_info_set;

            std::vector<unsigned char> output(chunk.memory, chunk.memory + chunk.size);

            using namespace nlohmann;

            const json root = json::parse(output);

            const auto builds = root["builds"];

            for (const auto &build : builds)
            {
                const auto state = build["state"];
                const auto committed_at = build["commit"]["committed_at"];
                const auto repo_name = build["repository"]["name"];

                // Converting ISO8601 string to std::tm to std::time_t (unix time)
                int y,M,d,h,m;
                float s;

                sscanf(std::string(committed_at).c_str(), 
                    "%d-%d-%dT%d:%d:%fZ", 
                    &y, &M, &d, &h, &m, &s);

                std::tm time;
    
                time.tm_year = y - 1900; // Year since 1900
                time.tm_mon = M - 1; // 0-11
                time.tm_mday = d; // 1-31
                time.tm_hour = h; // 0-23
                time.tm_min = m; // 0-59
                time.tm_sec = static_cast<int>(s); // 0-60

                //std::cout << std::string(repo_name) << ": " << std::string(committed_at) << "\n";

                std::time_t unix_time = std::mktime(&time);

                // Insert this data IF it is more recent than what is already there
                auto search = build_info_set.find(std::string(repo_name)); 

                if (search == build_info_set.end() || search->second.time < unix_time)
                {
                    build_info_set.insert({std::string(repo_name),
                        {
                            std::string(state) != "failed",

                            unix_time
                        }
                    });
                }
            }

            /*for (auto &build : build_info_set)
            {
                std::cout << build.first << ", ";
                std::cout << build.second.time << "\n";
            }*/
        }
        else throw std::runtime_error(std::string("BLAR")
            .append("curl_easy_perform failed: ")
            .append(curl_easy_strerror(curlResult)));

        curl_easy_cleanup(curl_handle);

        free(chunk.memory);
    }
    else throw std::runtime_error(std::string("BLAR").append("Failed to initialize a curl session."));

    curl_global_cleanup();
}

////
std::string aTravisToken;

static bool update(GtkStatusIcon *tray_icon)
{
    if (!tray_icon) return false;

    do_request(aTravisToken);

    std::cout << "ping\n";

    return true;
}

static void icon_clicked()
{
    std::cout << "clicked\n";

    system("xdg-open https://travis-ci.org/github/jfcameron");
}

static void create_tray_icon()
{
    GtkStatusIcon *tray_icon = gtk_status_icon_new();

    auto pixbuf = jfc::get_error_icon_image(); 

    gtk_status_icon_set_from_pixbuf(tray_icon, pixbuf);

    gtk_status_icon_set_tooltip_text (tray_icon, "travis-ci canary");
    gtk_status_icon_set_visible (tray_icon, TRUE);

    update(tray_icon);

    g_timeout_add_seconds(30,
        reinterpret_cast<GSourceFunc>(update),
        reinterpret_cast<gpointer>(tray_icon));

    g_signal_connect(G_OBJECT(tray_icon), 
        "activate", 
        G_CALLBACK(icon_clicked), 
        NULL);
}

void launch_application(GSimpleAction*, GVariant*, gpointer)
{
    std::cout << "whew\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) throw std::invalid_argument("need 1 arg: the travis token");

    aTravisToken = argv[1];

    gtk_init(&argc, &argv);

    create_tray_icon();

    //notification init
    auto app = g_application_new("jfcameron.github.travis_ci_canary", G_APPLICATION_FLAGS_NONE);
    g_application_register(app, nullptr, nullptr);

    static GActionEntry actions[] = {
        { "build.failed", launch_application}
    };

    g_action_map_add_action_entries (G_ACTION_MAP (app),
            actions, G_N_ELEMENTS (actions),
            app);

    GNotification *notification = g_notification_new("Build failed");
    g_notification_set_default_action(notification, "app.build.failed");


    //example notification sending
    g_notification_set_body(notification, "repo/slug");
    g_application_send_notification(app, "blar", notification);

    g_notification_set_body(notification, "goober");
    g_application_send_notification(app, "blar", notification);
    
    gtk_main();

    return EXIT_SUCCESS;
}

