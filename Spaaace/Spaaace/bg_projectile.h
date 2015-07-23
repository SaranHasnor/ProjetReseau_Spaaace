#pragma once

typedef struct {
	int		ownerID;

	float	pos[3];
	float	ang[3];
	float	vel[3];

	float	*target[3];

	int		damage;
} projectile_t;


projectile_t *BG_newProjectile(int ownerID, int damage);
float BG_projectileDistanceToPoint(projectile_t *projectile, float pos[3]);
void BG_updateProjectile(projectile_t *projectile, float deltaTime);
