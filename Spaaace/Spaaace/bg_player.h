#pragma once

#ifndef _PLAYER_SPACE_DEFINED
#define _PLAYER_SPACE_DEFINED

#include <utils.h>

#include "bg_input.h"

typedef struct {
	playerInput_t	input;

    int				id;

    float			pos[3];
    float			vel[3];
	float			ang[3];

    uint			health;
    int				score;
} player_t;

void BG_initPlayer(player_t *player, int id);
void BG_playerTakeDamage(player_t *player, uint damage);
void BG_updatePlayer(player_t *player, float deltaTime);
void BG_serializePlayer(player_t *in, bytestream *out);
void BG_deserializePlayer(bytestream *in, player_t *out);

#endif