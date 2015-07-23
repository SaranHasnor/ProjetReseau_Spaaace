#include "bg_player.h"

#include <utils_vector.h>

const int _maxHealth = 100;

void BG_initPlayer(player_t *player, int id)
{
    mem_set(player, 0, sizeof(player_t));

	player->id = id;
	player->health = _maxHealth;
}

void BG_playerTakeDamage(player_t *player, uint damage)
{
	if (damage > player->health)
	{
		player->health = 0;
	}
	else
	{
		player->health -= damage;
	}
}

void BG_updatePlayer(player_t *player, float deltaTime)
{
    float velocity[3] = { 0, 0, 0 };
	float forward[3], right[3];

	// First read his input
    if (player->input.forward)
    {
        velocity[1] += 1;
    }

    if (player->input.back)
    {
        velocity[1] -= 1;
    }

    if (player->input.right)
    {
        velocity[0] += 1;
    }

    if (player->input.left)
    {
        velocity[0] -= 1;
    }

    if (player->input.up)
    {
         velocity[2] += 1;
    }

    if (player->input.down)
    {
        velocity[2] -= 1;
    }

	player->ang[0] += player->input.angleDelta[0];
	player->ang[1] += player->input.angleDelta[1];

	if (player->ang[0] < -85)
		player->ang[0] = -85;
	if (player->ang[0] > 85)
		player->ang[0] = 85;

	if (player->ang[1] < -180)
		player->ang[1] += 360;
	if (player->ang[1] > 180)
		player->ang[1] -= 360;

	// Now map his movement to his local axis (except for the Z axis)
	AngleVectors(player->ang, forward, right, NULL);
	vectorScale(player->vel, velocity[2], axis[2]);
	vectorMA(player->vel, player->vel, velocity[1], forward);
	vectorMA(player->vel, player->vel, velocity[0], right);

	// And finally move him
	vectorMA(player->pos, player->pos, deltaTime, player->vel);
}

void BG_serializePlayer(player_t *in, bytestream* out)
{
    bytestream_write(out, (byte*)in, sizeof(player_t));
}

void BG_deserializePlayer(bytestream *in, player_t *out)
{
    bytestream_read(in, (byte*)out, sizeof(player_t));
}