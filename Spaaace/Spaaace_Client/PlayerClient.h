#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <engine_render.h>
#include <network_client.h>

typedef struct {
    SpacePlayer_t PlayerId;
    mesh_t* PlayerMesh;
} ClientPlayer_t;

ClientPlayer_t* ClientPlayerList;

void ClientPlayerInitialize();
void CreateNewPlayer(bool isMine);
void PlayerWantToMoveTo(float posX, float posY, float posZ);


#endif
