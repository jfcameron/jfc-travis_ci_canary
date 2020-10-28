// \brief a temporary file containing global code, used to aid in refactor.

// © 2020 Joseph Cameron - All Rights Reserved

#ifndef DELTE_MEDELETEME
#define DELTE_MEDELETEME

#include <string>

enum class state_type
{
    succeeded,
    failed,
    cancelled,
    building,
    disconnected // ?
};

enum class connection_state_type
{
    connected,
    disconnected
};

void set_tray_icon(state_type state);
void set_icon_tooltip(const std::string &aToolTip);

#endif

