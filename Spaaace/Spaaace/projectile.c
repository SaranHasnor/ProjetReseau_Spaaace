#include "projectile.h"

#include <utils.h>
#include <utils_vector.h>


projectile_t *newProjectile()
{
	projectile_t *proj = (projectile_t*)mem_alloc(sizeof(projectile_t));
	mem_set(proj, 0, sizeof(projectile_t));
	return proj;
}

float projectileDistanceToPoint(projectile_t *projectile, float pos[3])
{
	return vectorDistance(projectile->pos, pos);
}

void updateProjectile(projectile_t *projectile, float deltaTime)
{
	float velNorm[3];
	float speed;

	vectorCopy(velNorm, projectile->vel);
	speed = vectorNormalize(velNorm);

	if (projectile->target)
	{
		float dir[3];
		vectorSubtract(dir, *projectile->target, projectile->pos);
		vectorScale(projectile->vel, speed, dir);
	}

	vectorMA(projectile->pos, projectile->pos, deltaTime, projectile->vel);
	
	vectoangles(velNorm, projectile->ang);
}