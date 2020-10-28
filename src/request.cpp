// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/travis_ci_canary/request.h>

#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <vector>

#include <curl/curl.h>

using namespace jfc::travis_ci_canary::request;

// buffer used to store binary data fetched from remote server
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
            throw std::runtime_error(std::string(
                "gdk::resources::remote fetch failed: could not allocate system memory to store fetched content!"));

        std::memcpy(&(pResponseBuffer->memory[pResponseBuffer->size]), contentPointer, contentByteCount);

        pResponseBuffer->size += contentByteCount;

        pResponseBuffer->memory[pResponseBuffer->size] = {0};

        return contentByteCount;
    }();
    
    return 0;
}

static CURL *curl_handle;

static void init_once()
{
    static bool once(true);

    if (once)
    {
        curl_global_init(CURL_GLOBAL_ALL);
    
        curl_handle = curl_easy_init();

        once = false;
    }
}

void jfc::travis_ci_canary::request::builds(std::string aTravisToken, 
    jfc::travis_ci_canary::request::response_handler_type aHandler,
    jfc::travis_ci_canary::request::failed_handler_type aFailedHandler)
{
    init_once();

    std::string aURL("https://api.travis-ci.org/builds?limit=25&sort_by=started_at:desc");

    if (curl_handle)
    {
        struct MemoryStruct chunk = (MemoryStruct)
        {
            .memory = []()
            {
                if (auto pHeap = static_cast<char *>(std::malloc(1))) return pHeap;
                else throw std::runtime_error("could not allocate space on the heap");
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
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, 
            reinterpret_cast<void *>(&chunk)); // send the memory buffer to the function

        // perform request
        const CURLcode curlResult = curl_easy_perform(curl_handle);

        // call appropriate response
        if (curlResult == CURLE_OK)
            aHandler(std::vector<unsigned char>(chunk.memory, chunk.memory + chunk.size));
        else aFailedHandler();

        free(chunk.memory);
    }
    else throw std::runtime_error("Failed to initialize a curl session.");
}

