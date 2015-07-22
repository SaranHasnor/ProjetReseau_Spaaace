#include "Player.h"

#include <utils_vector.h>

float MaxLife = 100;

void ChangeMaxLife(float maxLife)
{
    MaxLife = maxLife;
}

void InitPlayer(SpacePlayer_t *player, int id)
{
	player->Id = id;
	vectorCopy(player->Position, nullVec);
	vectorCopy(player->Velocity, nullVec);
	vectorCopy(player->Angles, nullVec);
	player->Life = MaxLife;
	player->Kill = 0;
	player->Death = 0;
    mem_set(&player->input, 0, sizeof(PlayerInput_t));
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
	float forward[3], right[3];
    if (Player->input.UpButton)
    {
        velocity[1] += 1;
    }

    if (Player->input.DownButton)
    {
        velocity[1] -= 1;
    }

    if (Player->input.RightButton)
    {
        velocity[0] += 1;
    }

    if (Player->input.LeftButton)
    {
        velocity[0] -= 1;
    }

    if (Player->input.UpperButton)
    {
         velocity[2] += 1;
    }

    if (Player->input.DownerButton)
    {
        velocity[2] -= 1;
    }

	AngleVectors(Player->Angles, forward, right, NULL);

	vectorScale(Player->Velocity, velocity[2], axis[2]);
	vectorMA(Player->Velocity, Player->Velocity, velocity[1], forward);
	vectorMA(Player->Velocity, Player->Velocity, velocity[0], right);

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