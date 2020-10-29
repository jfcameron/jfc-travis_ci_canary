// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_CONFIG_H
#define JFC_TRAVIS_CI_CANARY_CONFIG_H

#include <string>

//TODO: not consts, load from a conf.
// if conf file does not exist, try to write one.
namespace jfc::travis_ci_canary::config
{
    void try_load_config_file();

    void save_config_file();

    std::string get_travis_token();

    std::string get_account_name();

    std::string get_api_root();

    std::string get_browser_command();
}

#endif

