#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

typedef struct {
    int Id;
    float PositionX;
    float PositionY;
    float PositionZ;
    float Speed;
    float Life;
    int Death;
    int Kill;
} SpacePlayer_t;

void Initialize();
void CreatePlayer(float posX, float posY, float posZ, bool isMine, int* outId);
void SetPlayerPosition(int playerId, float posX, float posY, float posZ);
void PlayerTakeDamage(int PlayerId, float AttackValue, bool *outIsInLife);

#endif