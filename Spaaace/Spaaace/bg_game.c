#include "bg_game.h"

#include <utils_vector.h>

gamedata_t game;

const float _playerHitboxSize = 1.0f;


void BG_gameLoop(float deltaTime)
{
	uint i, j;

	for (i = 0; i < game.players.size; i++)
	{
		player_t *player = (player_t*)game.players.content[i];

		BG_updatePlayer(player, deltaTime);

		if (player->input.attack)
		{
			float forward[3];
			projectile_t *proj = BG_newProjectile(player->id, 15);
			AngleVectors(player->ang, forward, NULL, NULL);
			vectorScale(proj->vel, 10, forward);
			vectorCopy(proj->pos, player->pos);
			vectorCopy(proj->ang, player->ang);
            list_add(&game.projectiles, proj);
		}
	}

	for (i = 0; i < game.projectiles.size; i++)
	{
		projectile_t *proj = (projectile_t*)game.projectiles.content[i];

		BG_updateProjectile(proj, deltaTime);

		for (j = 0; j < game.players.size; j++)
		{
			player_t *player = (player_t*)game.players.content[j];

			if (player->id != proj->ownerID && BG_projectileDistanceToPoint(proj, player->pos) < _playerHitboxSize)
			{
				BG_playerTakeDamage(player, proj->damage);
				list_removeAt(&game.projectiles, i--);
				break;
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
