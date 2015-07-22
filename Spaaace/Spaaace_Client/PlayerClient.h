#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <engine_render.h>
#include <network_client.h>

typedef struct {
    SpacePlayer_t BasePlayer;

    mesh_t* PlayerMesh;
} ClientPlayer_t;



void PlayerClient_init();
void RenderClient(float viewMatrix[16]);
void CreateNewPlayerStringMessage(string message);
void CreateNewPlayer(float position[3], int kill, int death);
void MovePlayer(float position[3], int PlayerId);
void MovePlayerMessage(string message);
void PlayerWantToMove(float position[3]);


#endif
