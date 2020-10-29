// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/travis_ci_canary/config.h>

#include <travisci_canary/buildinfo.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

using namespace jfc::travis_ci_canary;

namespace fs = std::filesystem;

static std::string travis_token = "";

static std::string account_name = "";

// Try to give a reasonable default browser command. 
static std::string browser_command = 
#if defined JFC_TARGET_PLATFORM_Linux
    "xdg-open";
#elif defined JFC_TARGET_PLATFORM_Darwin
    "open";
#else
    "firefox";
#endif

std::string jfc::travis_ci_canary::config::get_travis_token()
{
    return travis_token;
}

std::string jfc::travis_ci_canary::config::get_account_name()
{
    return account_name;
}

std::string jfc::travis_ci_canary::config::get_browser_command()
{
    return browser_command;
}

// Path to the config file will vary if non POSIX platforms are supported
const std::string config_directory =
#if defined JFC_TARGET_PLATFORM_Linux || defined JFC_TARGET_PLATFORM_Darwin //...
    std::string(std::getenv("HOME")).append("/.config/travis_ci_canary/");
//#elif defined JFC_TARGET_PLATFORM_Windows
    //windows api to get user's appdata path...
#else
#error "unsupported platform"
#endif

static const std::string config_filename("conf.json");

static const std::string config_path(config_directory + config_filename);
    
using namespace nlohmann;

void jfc::travis_ci_canary::config::load_config_file()
{
    if (!fs::exists(config_path)) 
    {
        std::cout << "Creating config file: \"" + config_path + "\n";

        save_config_file();
    }

    std::ifstream ifs(config_path);

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    try
    {
        auto root = json::parse(content);

        // Deserialization
        travis_token = root["travis_token"];
        account_name = root["account_name"];
        browser_command = root["browser_command"];

        std::string error_string;

        if (travis_token.empty()) error_string += "Config file does not contain a travis token. "
            "A token is required in order to list your travis-ci builds. "
            "A token can be generated after logging into the travis-ci website. "
            "Please add valid a token to the config file.";

        if (account_name.empty()) error_string += std::string(error_string.empty() ? "" : "\n") + 
            "Config file does not contain your account name. "
            "The account name is required to determine the URL to your recent builds. "
            "Please add your account name to the config file.";

        if (!error_string.empty()) throw std::invalid_argument(error_string);
    }
    catch (nlohmann::detail::parse_error e)
    {
        throw std::invalid_argument(std::string("Parsing error while loading the config file.")
            .append(" Path: {") + config_path + "}, Parsing error: {" + e.what() + "}");
    }
}

void jfc::travis_ci_canary::config::save_config_file()
{
    fs::create_directories(config_directory);
    
    std::ofstream ofs(config_path);

    json root;

    // Serialization
    root["travis_token"] = travis_token;
    root["account_name"] = account_name;
    root["browser_command"] = browser_command;

    ofs << root.dump(4, ' ');
}

