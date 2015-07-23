#include "bg_game.h"

#include <utils_vector.h>
#include <utils_time.h>

gamedata_t game;

const float _playerHitboxSize = 1.0f;
const float _projSpeed = 50.0f;
const int _projDamage = 15;
const long _attackDelay = 500;
const long _projLife = 5000;


void BG_gameLoop(float deltaTime)
{
	uint i, j;

	for (i = 0; i < game.players.size; i++)
	{
		player_t *player = (player_t*)game.players.content[i];

		BG_updatePlayer(player, deltaTime);

		if (player->input.attack && time_current_ms() > player->lastShotTime + _attackDelay)
		{
			float forward[3];
			projectile_t *proj = BG_newProjectile(player->id, _projDamage);
			AngleVectors(player->ang, forward, NULL, NULL);
			vectorScale(proj->vel, _projSpeed, forward);
			vectorMA(proj->pos, player->pos, 1.0f, forward);
			vectorCopy(proj->ang, player->ang);
            list_add(&game.projectiles, proj);
			player->lastShotTime = time_current_ms();
		}
	}

	for (i = 0; i < game.projectiles.size; i++)
	{
		projectile_t *proj = (projectile_t*)game.projectiles.content[i];

		if (time_current_ms() - proj->spawnTime > _projLife)
		{ // Timed out
			list_removeAt(&game.projectiles, i--);
			mem_free(proj);
		}
		else
		{
			BG_updateProjectile(proj, deltaTime);

			for (j = 0; j < game.players.size; j++)
			{
				player_t *player = (player_t*)game.players.content[j];

				if (player->id != proj->ownerID && BG_projectileDistanceToPoint(proj, player->pos) < _playerHitboxSize)
				{
					BG_playerTakeDamage(player, proj->damage);
					list_removeAt(&game.projectiles, i--);
					mem_free(proj);
					break;
				}
			}
		}
	}
}

player_t *BG_getPlayerWithID(int id)
{
	uint i;
	for (i = 0; i < game.players.size; i++)
	{
		player_t *player = (player_t*)game.players.content[i];
		if (player->id == id)
		{
			return player;
		}
	}
	return NULL;
}
