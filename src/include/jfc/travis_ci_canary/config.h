// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_CONFIG_H
#define JFC_TRAVIS_CI_CANARY_CONFIG_H

#include <string>

/// \brief get_ functions return values used throughout
/// the program. These values may be modifed by the end user
/// by editing the conf.json configuration file, which represents
/// the serialized form of this data
///
/// \warn load_config_file must be called before get_ 
/// otherwise only default values will be available.
/// This is fine in the case of optoinal values, but
/// will cause an exception for required values (e.g: travis api toke)
namespace jfc::travis_ci_canary::config
{
    /// \brief read values from the config file
    void load_config_file();

    /// \brief write values to the config file
    void save_config_file();

    /// \brief get a copy of the travis token
    std::string get_travis_token();

    /// \brief get a copy of the account name
    std::string get_account_name();

    /// \brief get a copy of the account name
    std::string get_browser_command();
}

#endif

