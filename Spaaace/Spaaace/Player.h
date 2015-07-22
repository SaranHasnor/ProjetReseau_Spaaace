#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

#include "PlayerInput.h"

typedef struct {
    int Id;
    float Position[3];
    float Velocity[3];
	float Angles[3];
    float Life;
    int Death;
    int Kill;

	PlayerInput_t input;
} SpacePlayer_t;

void InitPlayer(SpacePlayer_t *player);
void SetPlayerPosition(SpacePlayer_t* player, float position[3]);
bool PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue);
void UpdatePlayer(SpacePlayer_t* Player, float deltaTime);
void Player_Serialize(SpacePlayer_t player, bytestream* stream);
SpacePlayer_t* Player_Deserialize(bytestream stream);

#endif