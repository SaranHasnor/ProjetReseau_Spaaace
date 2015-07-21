#include "Player.h"

float Speed = 1.0f;
float MaxLife = 100;
unsigned int _MaxPlayer = 2;

int currentId = 0;

void ChangeMaxLife(float maxLife)
{
    MaxLife = maxLife;
}

void CreatePlayer(float Position[3], int kill, int death )
{
    SpacePlayer_t newPlayer;

    newPlayer.Id = currentId;
    newPlayer.Position[0] = Position[0];
    newPlayer.Position[1] = Position[1];
    newPlayer.Position[2] = Position[2];
    newPlayer.Speed = Speed;
    newPlayer.Life = MaxLife;
    newPlayer.Kill = 0;
    newPlayer.Death = 0;

    currentId++;
}

void SetPlayerPosition(SpacePlayer_t* Player, float Position[3])
{
    Player->Position[0] = Position[0];
    Player->Position[1] = Position[1];
    Player->Position[2] = Position[2];
}

void PlayerTakeDamage(SpacePlayer_t* Player, float AttackValue, bool* outIsInLife)
{
    Player->Life -= AttackValue;
    outIsInLife = Player->Life < 1;
}