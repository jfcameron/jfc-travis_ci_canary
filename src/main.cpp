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

// prog param. once ooed, pass this around on stack, dont use a global.
std::string aTravisToken;

enum class state_type
{
    succeeded,
    failed,
    cancelled
};

static void set_tray_icon(state_type state);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTIFICATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO: not an object. decompose. move work to cpp file, header <jfc/canary/notification.h>
namespace jfc::travis_ci_canary
{
    /// \brief notification facility for the project
    class notifier final
    {
        GApplication *m_app;

        GNotification *m_notification;
    public:
        /// \brief notify the user of a build failure for a given repo
        void notify(const std::string &aRepoURL, state_type state);

        /// \brief ctor
        notifier();
    };
}

using namespace jfc::travis_ci_canary;

void build_failed_notification_behaviour(GSimpleAction*, GVariant*, gpointer)
{
    std::cout << "whew\n";
}

notifier::notifier()
: m_app(g_application_new("jfcameron.github.travis_ci_canary", G_APPLICATION_FLAGS_NONE))
, m_notification(g_notification_new("Build failed"))
{
    g_application_register(m_app, nullptr, nullptr);

    static const GActionEntry ACTIONS[] = {
        { "build.failed", build_failed_notification_behaviour}
    };

    g_action_map_add_action_entries (G_ACTION_MAP (m_app), ACTIONS, 
        G_N_ELEMENTS (ACTIONS), m_app);
    
    g_notification_set_default_action(m_notification, "app.build.failed");
}

void notifier::notify(const std::string &aSlug, state_type state)
{
    switch (state)
    {
        case state_type::succeeded: g_notification_set_title(m_notification, "succeeded"); break;
        case state_type::cancelled: g_notification_set_title(m_notification, "cancelled"); break;
        case state_type::failed: g_notification_set_title(m_notification, "failed"); break;
    }

    g_notification_set_body(m_notification, aSlug.c_str());

    g_application_send_notification(m_app, "notification", m_notification);
}

jfc::travis_ci_canary::notifier *aNotifier;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FETCH
////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    std::string aURL("https://api.travis-ci.org/builds?limit=100&sort_by=started_at:desc");

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
                state_type state;
                state_type previous_state;

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
                const auto previous_state = build["previous_state"].is_string()
                    ? build["previous_state"]
                    : state;
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

                std::time_t unix_time = std::mktime(&time);

                // Insert this data IF it is more recent than what is already there
                auto search = build_info_set.find(std::string(repo_name)); 

                if (search == build_info_set.end() || search->second.time < unix_time)
                {
                    build_info_set[std::string(repo_name)] = {
                        [](std::string aState)
                        {
                            if (aState == "passed") return state_type::succeeded;
                            if (aState == "failed") return state_type::failed;
                            if (aState == "cancelled") return state_type::cancelled;
                            if (aState == "canceled") return state_type::cancelled;

                            throw std::invalid_argument(std::string("unhandled state type: ").append(aState));
                        }(std::string(state)),

                        [](std::string aState)
                        {
                            if (aState == "passed") return state_type::succeeded;
                            if (aState == "failed") return state_type::failed;
                            if (aState == "cancelled") return state_type::cancelled;
                            if (aState == "canceled") return state_type::cancelled;

                            throw std::invalid_argument(std::string("unhandled state type: ").append(aState));
                        }(std::string(state)),//previous_state)),

                        unix_time
                    };
                }
            }

            //visitor functor?
            {
                auto state = state_type::failed;

                for (const auto &[key, value] : build_info_set)
                {
                    state = value.state;
                
                    if (value.state != value.previous_state) aNotifier->notify(key, value.state);
                }
                
                set_tray_icon(state);
            }
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SYSICON
////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

GtkStatusIcon *tray_icon;

static void create_tray_icon()
{
    tray_icon = gtk_status_icon_new();

    set_tray_icon(state_type::failed);

    gtk_status_icon_set_tooltip_text (tray_icon, "travis-ci canary");
    gtk_status_icon_set_visible (tray_icon, TRUE);

    update(tray_icon);

    g_timeout_add_seconds(15,
        reinterpret_cast<GSourceFunc>(update),
        reinterpret_cast<gpointer>(tray_icon));

    g_signal_connect(G_OBJECT(tray_icon), 
        "activate", 
        G_CALLBACK(icon_clicked), 
        NULL);
}

static void set_tray_icon(state_type state)
{
    switch (state)
    {
        case(state_type::succeeded): gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_ok_icon_image()); break;
        case(state_type::cancelled): gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_ok_icon_image()); break;
        case(state_type::failed):    gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_error_icon_image()); break;

        default: throw std::invalid_argument("set_tray_icon, unhandled state");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROGRAM ENTRY
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    if (argc < 2) throw std::invalid_argument("need 1 arg: the travis token");

    aTravisToken = argv[1];

    gtk_init(&argc, &argv);

    aNotifier = new jfc::travis_ci_canary::notifier();

    create_tray_icon();
   
    gtk_main();

    return EXIT_SUCCESS;
}

