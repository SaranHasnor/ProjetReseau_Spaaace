#pragma once

#ifndef _PLAYER_INPUT_DEFINED
#define _PLAYER_INPUT_DEFINED
#include <utils.h>
#include <utils_bytestream.h>

typedef struct{
    bool UpButton;
    bool DownButton;
    bool LeftButton;
    bool RightButton;
    bool AttackButton;
}PlayerInput_t;

void PlayerInput_Serialize(PlayerInput_t playerInput, bytestream* stream);

#endif