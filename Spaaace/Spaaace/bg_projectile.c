#include "bg_projectile.h"

#include <utils.h>
#include <utils_vector.h>


projectile_t *BG_newProjectile(int ownerID, int damage)
{
	projectile_t *proj = (projectile_t*)mem_alloc(sizeof(projectile_t));
	mem_set(proj, 0, sizeof(projectile_t));
	proj->ownerID = ownerID;
	proj->damage = damage;
	return proj;
}

float BG_projectileDistanceToPoint(projectile_t *projectile, float pos[3])
{
	return vectorDistance(projectile->pos, pos);
}

void BG_updateProjectile(projectile_t *projectile, float deltaTime)
{
	float velNorm[3];
	float speed;

	vectorCopy(velNorm, projectile->vel);
	speed = vectorNormalize(velNorm);

	if (projectile->target && *projectile->target)
	{
		float dir[3];
		vectorSubtract(dir, *projectile->target, projectile->pos);
		vectorScale(projectile->vel, speed, dir);
	}

	vectorMA(projectile->pos, projectile->pos, deltaTime, projectile->vel);
	
	vectoangles(velNorm, projectile->ang);
}