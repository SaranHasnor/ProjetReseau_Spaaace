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
    bool UpperButton;
    bool DownerButton;
    bool AttackButton;
}PlayerInput_t;

void PlayerInput_Serialize(PlayerInput_t playerInput, bytestream* stream);
void PlayerInput_Deserialize(bytestream stream, PlayerInput_t* playerInput);

#endif