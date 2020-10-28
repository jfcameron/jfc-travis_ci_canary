// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY
#define JFC_TRAVIS_CI_CANARY

#include <gdk-pixbuf/gdk-pixbuf.h>

namespace jfc
{
    GdkPixbuf *get_error_icon_image();

    GdkPixbuf *get_ok_icon_image();
    
    GdkPixbuf *get_building_icon_image();

    GdkPixbuf *get_disconnected_icon_image();
}

#endif

