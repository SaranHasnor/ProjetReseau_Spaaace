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
    float velocity[3] = { 0, 0, 0 };
    if (Player->input.UpButton)
    {
        velocity[0] += 1;
    }

    if (Player->input.DownButton)
    {
        velocity[0] -= 1;
    }

    if (Player->input.LeftButton)
    {
        velocity[1] -= 1;
    }

    if (Player->input.UpperButton)
    {
         velocity[2] += 1;
    }

    if (Player->input.DownerButton)
    {
        velocity[2] -= 1;
    }

    vectorCopy(&Player->Velocity, velocity);

	vectorMA(Player->Position, Player->Position, deltaTime, Player->Velocity);
}

void Player_Serialize(SpacePlayer_t player, bytestream* stream)
{
    bytestream_write(stream, (byte*)&player, sizeof(SpacePlayer_t));
}

void Player_Deserialize(bytestream stream, SpacePlayer_t *out)
{
    bytestream_read(&stream, (byte*)out, sizeof(SpacePlayer_t));
}