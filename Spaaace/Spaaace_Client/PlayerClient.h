#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <engine_render.h>

SpacePlayer_t* myPlayer;

void CreatePlayerMesh();
void RenderPlayer(SpacePlayer_t* player, float viewMatrix[16]);
SpacePlayer_t* CreateNewPlayer(bytestream message);

#endif
