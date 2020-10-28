// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_ICON_H
#define JFC_TRAVIS_CI_CANARY_ICON_H

#include <jfc/travis_ci_canary/enums.h>

#include <string>

/// \brief functions used to manipulate the system tray icon
///
/// \warn gtk_init must be called before using any function declared in this namespace
namespace jfc::travis_ci_canary::icon
{
    /// \brief changes icon graphic given a build state TODO: rename "state" to something more descriptive
    void set_tray_icon(build_state_type state);
   
    /// \brief changes icon graphic given a connection state
    void set_tray_icon(connection_state_type state);

    void set_default_icon();

    /// \brief changes the icon tool tip. 
    ///
    /// \note tool tip is displayed when user holds the cursor over the icon
    void set_icon_tooltip(const std::string &aToolTip);
}

#endif

