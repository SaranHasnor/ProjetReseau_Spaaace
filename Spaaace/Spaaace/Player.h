#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

typedef struct {
    int Id;
    float Position[3];
    float Velocity[3];
	float Angles[3];
    float Life;
    int Death;
    int Kill;
} SpacePlayer_t;

SpacePlayer_t *CreatePlayer(float Position[3], int kill, int death);
void SetPlayerPosition(SpacePlayer_t* player, float position[3]);
bool PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue);
void UpdatePlayer(SpacePlayer_t* Player, float deltaTime);

#endif