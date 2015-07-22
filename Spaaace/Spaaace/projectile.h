
typedef struct {
	int		ownerID;

	float	pos[3];
	float	ang[3];
	float	vel[3];

	float	*target[3];

	float		damage;
} projectile_t;


projectile_t *newProjectile(int ownerID, float damage);
float projectileDistanceToPoint(projectile_t *projectile, float pos[3]);
void updateProjectile(projectile_t *projectile, float deltaTime);
