#pragma once

#ifndef _PLAYER_INPUT_DEFINED
#define _PLAYER_INPUT_DEFINED
#include <utils.h>
#include <utils_bytestream.h>

typedef struct {
    bool forward;
    bool back;
    bool left;
    bool right;
    bool up;
    bool down;
    bool attack;
} playerInput_t;

void BG_serializePlayerInput(playerInput_t *in, bytestream *out);
void BG_deserializePlayerInput(bytestream *in, playerInput_t *out);

#endif