// © 2020 Joseph Cameron - All Rights Reserved
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

#include <gtk/gtk.h>
#include <nlohmann/json.hpp>

#include <jfc/travis_ci_canary/config.h>
#include <jfc/travis_ci_canary/enums.h>
#include <jfc/travis_ci_canary/icon.h>
#include <jfc/travis_ci_canary/notification.h>
#include <jfc/travis_ci_canary/request.h>

#include <travisci_canary/buildinfo.h>

/// \brief converts a string containing ISO8601 formatted date to a std::time_t (epoch)
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

    if (!builds.is_array()) throw std::runtime_error("travis response malformed: does not contain builds array");

    for (const auto &build : builds)
    {
        const auto state = build["state"].is_string()
            ? std::string(build["state"]) 
            : "failed";

        const auto time_stamp = [&json_time = build["updated_at"]]()
        {
            if (!json_time.is_string()) throw std::runtime_error("travis response malformed: "
                "build does not contain required timestamp");

            return json_time;
        }();

        if (!build["commit"].is_object()) 
            throw std::runtime_error("travis response malformed: build does not contain commit object");

        const auto commit_sha = build["commit"]["sha"].is_string()
            ? std::string(build["commit"]["sha"])
            : "unknown";

        if (!build["repository"].is_object()) 
            throw std::runtime_error("travis response malformed: build does not contain repository object");

        const auto repo_name = build["repository"]["name"].is_string()
            ? std::string(build["repository"]["name"])
            : "unknown";

        std::time_t unix_time = iso8601_to_time_t(time_stamp);
        
        auto search = current_build_info_set.find(repo_name); 
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

            // first entry case
            if (search == current_build_info_set.end()) 
            {
                current_build_info_set[repo_name] = newinfo;
            }
            // newer available: unfortunately the API very occasionally returns things out of order
            else if (search !=  current_build_info_set.end() && search->second.time < newinfo.time) 
            {
                current_build_info_set[repo_name] = newinfo;
            }
        }
    }

    build_state_type state = build_state_type::succeeded;

    for (const auto &[key, value] : current_build_info_set)
    {
        // Notifications
        if (auto last_search = last_build_info_set.find(key); last_search != last_build_info_set.end())
        {
            if (last_search->second.state != value.state) notify::build_state_changed(key, value.state);
        }

        // Icon graphic
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

    // Icon tooltip
    if (state == build_state_type::succeeded) 
        jfc::travis_ci_canary::icon::set_tooltip("all builds succeeded");
    else if (state == build_state_type::building) 
        jfc::travis_ci_canary::icon::set_tooltip("building");
    else if (state == build_state_type::failed) 
        jfc::travis_ci_canary::icon::set_tooltip("a build has failed");

    jfc::travis_ci_canary::icon::set_graphic(state);

    last_build_info_set = current_build_info_set;
}

void failed_handler()
{
    icon::set_graphic(connection_state_type::disconnected);

    icon::set_tooltip("disconnected");
}

bool update()
{
    request::builds(&response_handler, &failed_handler);

    return true;
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc <= 1)
        {
            config::try_load_config_file();

            gtk_init(&argc, &argv);

            icon::set_default_graphic();
            
            // this allows gtk_main to get called before update work starts, 
            // so the default icon will render immediately on application start
            g_timeout_add_seconds(1, [](void *const vp) 
                {
                    auto token = reinterpret_cast<std::string *>(vp);

                    update();
                    
                    g_timeout_add_seconds(15, reinterpret_cast<GSourceFunc>(update), nullptr);

                    return int(0);
                }, 
                nullptr);
           
            gtk_main();
        }
        else
        {
            std::vector<std::string> args(argv, argv + argc);

            if (args[1] == "-h" || args[1] == "--help")
            {
                std::cout << "git hash: " << travisci_canary_BuildInfo_Git_Commit 
                    << ", build date: " << travisci_canary_BuildInfo_Git_Date << "\n";
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "error: unhandled exception" << std::endl;
    }

    return EXIT_SUCCESS;
}

