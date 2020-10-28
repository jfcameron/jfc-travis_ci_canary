// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/travis_ci_canary/config.h>
#include <jfc/travis_ci_canary/request.h>

#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <vector>
#include <memory>
#include <iostream>
#include <iterator>

#include <curl/curl.h>

using namespace jfc::travis_ci_canary::request;
using namespace jfc::travis_ci_canary;

static size_t WriteMemoryCallback(unsigned char *const contentPointer, 
    const size_t contentItemSize, 
    const size_t contentItemCount, 
    void *const userPointer)
{
    const size_t contentByteCount(contentItemSize * contentItemCount);

    auto pResponseBuffer(static_cast<response_data_type *const>(userPointer));

    pResponseBuffer->insert(pResponseBuffer->end(), contentPointer, contentPointer + contentByteCount);

    return contentByteCount;
}

static CURL *curl_handle;

static void init_once()
{
    static bool once(true);

    if (once)
    {
        once = false;

        // Init libcurl
        curl_global_init(CURL_GLOBAL_ALL);
   
        // Create handle for builds request
        curl_handle = curl_easy_init();

        if (!curl_handle) throw std::runtime_error("Failed to initialize a curl session.");

        // Configure the builds request
        std::string aURL("https://api.travis-ci.org/builds?limit=25&sort_by=started_at:desc");

        curl_easy_setopt(curl_handle, CURLOPT_URL, aURL.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_slist *headerlist = curl_slist_append(nullptr, "Travis-API-Version: 3");
        headerlist = curl_slist_append(headerlist, 
            std::string(std::string("Authorization: token ") + config::aTravisToken).c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    }
}

void jfc::travis_ci_canary::request::builds(std::string aTravisToken, 
    jfc::travis_ci_canary::request::response_handler_type aHandler,
    jfc::travis_ci_canary::request::failed_handler_type aFailedHandler)
{
    init_once();

    response_data_type buffer;
    
    // provide a new memory buffer to the writefunction registered in init_once
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, reinterpret_cast<void *>(&buffer)); 

    // perform request
    const CURLcode curlResult = curl_easy_perform(curl_handle);

    // call appropriate response functor
    if (curlResult == CURLE_OK) aHandler(buffer);
    else aFailedHandler();
}

