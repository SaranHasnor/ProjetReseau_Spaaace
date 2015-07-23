#pragma once

#ifndef _PLAYER_SERVER_DEFINED
#define _PLAYER_SERVER_DEFINED

#include <bg_player.h>
#include <bg_game.h>
#include <utils_list.h>
#include <network_server.h>

typedef struct {
	SpacePlayer_t BasePlayer;

	bytestream connectionData;
} ServerPlayer_t;

ServerPlayer_t *CreateNewPlayer(int senderId);

#endif
