#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

typedef struct {
    int Id;
    float Position[3];
    float Speed;
    float Life;
    int Death;
    int Kill;
} SpacePlayer_t;

void InitializePlayerList();
void CreatePlayer(float Position[3], int kill, int death);
void SetPlayerPosition(int playerId, float posX, float posY, float posZ);
void PlayerTakeDamage(int PlayerId, float AttackValue, bool *outIsInLife);

#endif