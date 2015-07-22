#pragma once

#ifndef _PLAYER_SERVER_DEFINED
#define _PLAYER_SERVER_DEFINED

#include "Player.h"
#include <utils_list.h>
#include <network_server.h>

typedef struct {
	SpacePlayer_t BasePlayer;

    int NetworkId;
} ServerPlayer_t;

ServerPlayer_t *CreateNewPlayer(int senderId);

#endif


