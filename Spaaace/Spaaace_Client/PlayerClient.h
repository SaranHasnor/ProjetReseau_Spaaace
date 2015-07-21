#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <utils_list.h>
#include <engine_render.h>
#include <network_client.h>

typedef struct {
    SpacePlayer_t Player;
    mesh_t* PlayerMesh;
} ClientPlayer_t;

list_t ClientPlayerList;
int MyPlayerId;

void PlayerClient_init();
void CreateNewPlayerStringMessage(string message);
void CreateNewPlayer(float position[3], int kill, int death);
void PlayerWantToMoveTo(float position[3], int PlayerId);


#endif
