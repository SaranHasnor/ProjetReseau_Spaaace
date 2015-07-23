#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include <bg_player.h>
#include <engine_render.h>

typedef enum { UpButton, DownButton, LeftButton, RightButton, UpperButton, DownerButton, AttackButton } InputValue;

extern player_t *myPlayer;

void createPlayerMesh();
void renderPlayer(player_t* player, float viewMatrix[16]);
player_t* createNewPlayer(int id);

#endif
