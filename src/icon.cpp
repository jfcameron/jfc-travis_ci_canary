// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/travis_ci_canary/config.h>
#include <jfc/travis_ci_canary/icon.h>
#include <jfc/travis_ci_canary/icons.h>

#include <gtk/gtk.h>

#include <mutex>
#include <iostream>

using namespace jfc::travis_ci_canary;

static GtkStatusIcon *tray_icon;

static void init_once()
{
    static bool once(true);

    if (once)
    {
        once = false;

        tray_icon = gtk_status_icon_new();

        icon::set_graphic(build_state_type::failed);

        gtk_status_icon_set_visible (tray_icon, TRUE);

        g_signal_connect(G_OBJECT(tray_icon), 
            "activate", 
            G_CALLBACK([](){
                system(std::string(config::get_browser_command())
                    .append(" ")
                    .append("https://travis-ci.org/github/" + config::get_account_name())
                    .c_str());}),
            nullptr
        );
    }

    return;
}

void jfc::travis_ci_canary::icon::set_graphic(build_state_type state)
{
    init_once();

    switch (state)
    {
        case(build_state_type::failed): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_error_icon_image()); 
            break;

        case(build_state_type::building): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_building_icon_image()); 
            break;
        
        case(build_state_type::succeeded): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_ok_icon_image());
            break;

        default: throw std::invalid_argument("set_graphic, unhandled state");
    }
}

void jfc::travis_ci_canary::icon::set_graphic(connection_state_type state)
{
    init_once();
   
    switch (state)
    {
        case(connection_state_type::disconnected): 
            gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_disconnected_icon_image()); 
            return;
        
        case(connection_state_type::connected): return;
    }
}

void jfc::travis_ci_canary::icon::set_default_graphic()
{
    init_once();
    
    gtk_status_icon_set_from_pixbuf(tray_icon, jfc::get_init_icon_image()); 
}

void jfc::travis_ci_canary::icon::set_tooltip(const std::string &aToolTip)
{
    init_once();

    gtk_status_icon_set_tooltip_text(tray_icon, aToolTip.c_str());
}

