// © 2020 Joseph Cameron - All Rights Reserved
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

#include <gtk/gtk.h>
#include <nlohmann/json.hpp>

#include <jfc/travis_ci_canary/enums.h>
#include <jfc/travis_ci_canary/icon.h>
#include <jfc/travis_ci_canary/notification.h>
#include <jfc/travis_ci_canary/request.h>

/// \brief converts a string comtaining ISO8601 formatted date to a std::time_t (epoch)
std::time_t iso8601_to_time_t(const std::string &aDate)
{
    //TODO: throw if aDate format is not 8601

    int y,M,d,h,m;
    float s;

    sscanf(aDate.c_str(), 
        "%d-%d-%dT%d:%d:%fZ", 
        &y, &M, &d, &h, &m, &s);

    std::tm time;

    time.tm_year = y - 1900;           // Year since 1900
    time.tm_mon = M - 1;               // 0-11
    time.tm_mday = d;                  // 1-31
    time.tm_hour = h;                  // 0-23
    time.tm_min = m;                   // 0-59
    time.tm_sec = static_cast<int>(s); // 0-60

    std::time_t unix_time = std::mktime(&time);

    return unix_time;
}

using namespace jfc::travis_ci_canary;

struct build_info
{
    build_state_type state;
    
    std::time_t time;
};

std::unordered_map</*repo name*/std::string, build_info> last_build_info_set;

void response_handler(std::vector<unsigned char> output)
{
    std::unordered_map</*repo name*/std::string, build_info> current_build_info_set;

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

        std::time_t unix_time = iso8601_to_time_t(committed_at);

        if (auto search = current_build_info_set.find(repo_name); search == current_build_info_set.end())
        {
            build_info newinfo = {
                [](std::string aState)
                {
                    if (aState == "passed") return build_state_type::succeeded;
                    if (aState == "failed") return build_state_type::failed;
                    if (aState == "errored") return build_state_type::failed;
                    if (aState == "cancelled") return build_state_type::cancelled;
                    if (aState == "canceled") return build_state_type::cancelled;
                    if (aState == "created") return build_state_type::building;
                    if (aState == "started") return build_state_type::building;

                    throw std::invalid_argument(std::string("unhandled state type: ").append(aState));
                }(state),

                unix_time
            };

            current_build_info_set[repo_name] = newinfo;

            if (auto last_search = last_build_info_set.find(repo_name); last_search != last_build_info_set.end())
            {
                if (last_search->second.state != newinfo.state) notify::build_state_changed(repo_name, newinfo.state);
            }
        }
    }

    build_state_type state = build_state_type::succeeded;

    for (const auto &[key, value] : current_build_info_set)
    {
        if (value.state == build_state_type::building) 
        {
            state = build_state_type::building;
        }
        else if (value.state == build_state_type::failed) 
        {
            state = build_state_type::failed;

            break;
        }
    }

    if (state == build_state_type::succeeded) jfc::travis_ci_canary::icon::set_icon_tooltip("all builds succeeded");
    else if (state == build_state_type::building) jfc::travis_ci_canary::icon::set_icon_tooltip("building");
    else if (state == build_state_type::failed) jfc::travis_ci_canary::icon::set_icon_tooltip("a build has failed");

    jfc::travis_ci_canary::icon::set_tray_icon(state);

    last_build_info_set = current_build_info_set;
}

void failed_handler()
{
    icon::set_tray_icon(connection_state_type::disconnected);

    icon::set_icon_tooltip("disconnected");
}

bool update(std::string *aTravisToken)
{
    request::builds(*aTravisToken, &response_handler, &failed_handler);

    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2) throw std::invalid_argument("need 1 arg: the travis token");

    std::string travisToken = argv[1];

    gtk_init(&argc, &argv);

    icon::set_default_icon();
    
    // this allows gtk_main to get called before update work starts, 
    // so the default icon will render immediately on application start
    g_timeout_add_seconds(1, [](void *const vp) 
        {
            auto token = reinterpret_cast<std::string *>(vp);

            update(token);
            
            g_timeout_add_seconds(15, reinterpret_cast<GSourceFunc>(update), token);

            return int(0);
        }, 
        &travisToken);
   
    gtk_main();

    return EXIT_SUCCESS;
}

