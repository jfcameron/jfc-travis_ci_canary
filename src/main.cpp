// © 2020 Joseph Cameron - All Rights Reserved
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

#include <curl/curl.h>

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
    std::string aURL("https://api.travis-ci.org/builds");

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
            std::cout << "response is ok\n";

            std::vector<unsigned char> output(chunk.memory, chunk.memory + chunk.size);

            for (const auto a : output)
            {
                std::cout << a;
            }
        }
        else throw std::runtime_error(std::string("BLAR")
            .append("curl_easy_perform failed: ")
            .append(curl_easy_strerror(curlResult)));

        curl_easy_cleanup(curl_handle);

        free(chunk.memory);
    }
    else throw std::runtime_error(std::string("BLAR").append("Failed to initialize a curl session."));

    curl_global_cleanup();  // MOVE TO A CURL WRAPPER
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

int main(int argc, char *argv[])
{
    if (argc < 2) throw std::invalid_argument("need 1 arg: the travis token");

    aTravisToken = argv[1];

    gtk_init(&argc, &argv);

    create_tray_icon();

    gtk_main();

    return EXIT_SUCCESS;
}

