#include "game.h"

#include <utils_vector.h>

gamedata_t game;

const float _playerHitboxSize = 1.0f;


void updateGame(float deltaTime)
{
	uint i, j;

	for (i = 0; i < game.players.size; i++)
	{
		SpacePlayer_t *player = game.players.content[i];

		UpdatePlayer(player, deltaTime);
	}

	for (i = 0; i < game.projectiles.size; i++)
	{
		projectile_t *proj = game.projectiles.content[i];

		updateProjectile(proj, deltaTime);

		for (j = 0; j < game.players.size; j++)
		{
			SpacePlayer_t *player = game.players.content[j];

			if (player->Id != proj->ownerID && projectileDistanceToPoint(proj, player->Position) < _playerHitboxSize)
			{
				PlayerTakeDamage(player, proj->damage);
				list_removeAt(&game.projectiles, i--);
				break;
			}
		}
	}
}