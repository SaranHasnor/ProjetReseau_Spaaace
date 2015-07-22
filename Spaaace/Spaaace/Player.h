#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

#include "PlayerInput.h"

typedef struct {
	PlayerInput_t input;

    int Id;
    float Position[3];
    float Velocity[3];
	float Angles[3];
    float Life;
    int Death;
    int Kill;
} SpacePlayer_t;

void InitPlayer(SpacePlayer_t *player);
void SetPlayerPosition(SpacePlayer_t* player, float position[3]);
bool PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue);
void UpdatePlayer(SpacePlayer_t* Player, float deltaTime);

#endif