#include "Player.h"
#include "projectile.h"
#include <utils_list.h>

typedef struct {
	list_t		players;
	list_t		projectiles;
} gamedata_t;

extern gamedata_t game;

void updateGame(float deltaTime);


SpacePlayer_t *GetPlayerWithId(int playerId);