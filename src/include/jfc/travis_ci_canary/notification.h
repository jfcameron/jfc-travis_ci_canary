// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_NOTIFICATION_H
#define JFC_TRAVIS_CI_CANARY_NOTIFICATION_H

#include <jfc/travis_ci_canary/enums.h>

#include <string>

/// \brief functions used to communicate state changes to the user via notifications
///
/// \warn gtk_init must be called before using any function declared in this namespace
namespace jfc::travis_ci_canary::notify
{
    /// \brief notify that a build's state has changed. e.g: building to succeeded
    void build_state_changed(const std::string &aRepoURL, build_state_type state);

    /// \brief notify that the program has just lost/gained connection to travis api
    void connection_state_changed(connection_state_type state);
}

#endif

