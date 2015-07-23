#pragma once

#ifndef _PLAYER_SERVER_DEFINED
#define _PLAYER_SERVER_DEFINED

#include <bg_player.h>
#include <bg_game.h>
#include <utils_list.h>
#include <network_server.h>

typedef struct {
	player_t BasePlayer;

	bytestream connectionData;
} serverPlayer_t;

serverPlayer_t *createNewPlayer(int id);

#endif
