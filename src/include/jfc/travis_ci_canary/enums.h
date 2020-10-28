// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_ENUMS_H
#define JFC_TRAVIS_CI_CANARY_ENUMS_H

#include <string>

enum class build_state_type
{
    succeeded,
    failed,
    cancelled,
    building,
};

enum class connection_state_type
{
    connected,
    disconnected,
};

#endif

