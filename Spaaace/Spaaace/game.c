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

		if (player->input.AttackButton)
		{
			float forward[3];
			projectile_t *proj = newProjectile(player->Id, 5.0f);
			AngleVectors(player->Angles, forward, NULL, NULL);
			vectorScale(proj->vel, 1, forward);
			vectorCopy(proj->pos, player->Position);
		}
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

SpacePlayer_t *GetPlayerWithId(int playerId)
{
	for (uint i = 0; i < game.players.size; i++)
	{
		SpacePlayer_t *player = (SpacePlayer_t*)game.players.content[i];
		if (player->Id == playerId)
		{
			return player;
		}
	}
	return NULL;
}