#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <engine_render.h>
#include <network_client.h>

typedef struct {
    int PlayerId;
    int NetworkPlayerId;
    mesh_t* PlayerMesh;
} ClientPlayer_t;

void CreateNewPlayer(bool isMine);
void PlayerWantToMoveTo(float posX, float posY, float posZ);


#endif
