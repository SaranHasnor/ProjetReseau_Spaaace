#include "Player.h"

#include <utils_vector.h>

float MaxLife = 100;

int currentId = 0;

void ChangeMaxLife(float maxLife)
{
    MaxLife = maxLife;
}

void InitPlayer(SpacePlayer_t *player)
{
	player->Id = currentId++;
	vectorCopy(player->Position, nullVec);
	vectorCopy(player->Velocity, nullVec);
	vectorCopy(player->Angles, nullVec);
	player->Life = MaxLife;
	player->Kill = 0;
	player->Death = 0;
}

void SetPlayerPosition(SpacePlayer_t* Player, float Position[3])
{
	vectorCopy(Player->Position, Position);
}

bool PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue)
{ // Returns true if the player is still alive
    Player->Life -= AttackValue;
    return Player->Life < 1;
}

void UpdatePlayer(SpacePlayer_t* Player, float deltaTime)
{
	vectorMA(Player->Position, Player->Position, deltaTime, Player->Velocity);
}

void Player_Serialize(SpacePlayer_t player, bytestream* stream)
{
    bytestream_write(stream, (byte*)&player, sizeof(SpacePlayer_t));
}

SpacePlayer_t* Player_Deserialize(bytestream stream)
{
    SpacePlayer_t *player=(SpacePlayer_t*)mem_alloc(sizeof(SpacePlayer_t));
    bytestream_read(&stream, (byte*)player, sizeof(SpacePlayer_t));
    return player;
}