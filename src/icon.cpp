// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/travis_ci_canary/icon.h>
#include <jfc/travis_ci_canary/icons.h>

#include <gtk/gtk.h>

#include <mutex>
#include <iostream>

static GtkStatusIcon *tray_icon;

static void init_once()
{
    static bool once(true);

    if (once)
    {
        once = false;

        std::cout << "init\n";

        tray_icon = gtk_status_icon_new();

        jfc::travis_ci_canary::icon::set_tray_icon(state_type::failed);

        gtk_status_icon_set_visible (tray_icon, TRUE);

        g_signal_connect(G_OBJECT(tray_icon), 
            "activate", 
            G_CALLBACK([](){
//Either param this at build time like what follows, or load the program name from a config file, with a reasonable default
// based on the compiler used
//#ifdef LAUNCH_OPTION_XDG_OPEN
                system("xdg-open https://travis-ci.org/github/jfcameron");
//#else if LAUNCH_OPTION_APPLE_OPEN
// sysy("open ...
//#else ....
//#endif
            }),
            nullptr
        );
    }

    return;
}

void jfc::travis_ci_canary::icon::set_tray_icon(state_type state)
{
    init_once();
    
    std::cout << "set_tray_icon\n";

    switch (state)
    {
        case(state_type::failed): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_error_icon_image()); 
            break;

        case(state_type::building): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_building_icon_image()); 
            break;
        
        case(state_type::succeeded): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_ok_icon_image());
            break;

        case(state_type::disconnected): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_disconnected_icon_image()); 
            break;

        default: throw std::invalid_argument("set_tray_icon, unhandled state");
    }
}

void jfc::travis_ci_canary::icon::set_icon_tooltip(const std::string &aToolTip)
{
    init_once();

    gtk_status_icon_set_tooltip_text(tray_icon, aToolTip.c_str());
}

