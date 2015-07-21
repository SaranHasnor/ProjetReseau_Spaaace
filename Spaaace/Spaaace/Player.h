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

void CreatePlayer(float Position[3], int kill, int death, SpacePlayer_t* outSpacePlayer);
void SetPlayerPosition(SpacePlayer_t* player, float position[3]);
void PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue, bool *outIsInLife);

#endif