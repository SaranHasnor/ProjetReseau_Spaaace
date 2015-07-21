#include "Player.h"

float Speed = 1.0f;
float MaxLife = 100;
unsigned int newPlayerIndex = 0;
unsigned int _MaxPlayer = 2;
SpacePlayer_t* PlayerList;

void PlayerList_init()
{
    PlayerList = (SpacePlayer_t*)mem_alloc(sizeof(SpacePlayer_t) * _MaxPlayer);
}

void ChangeMaxLife(float maxLife)
{
    MaxLife = maxLife;
}

void CreatePlayer(float Position[3], int kill, int death )
{
    SpacePlayer_t newPlayer;

    PlayerList[newPlayerIndex].Id = newPlayerIndex;
    PlayerList[newPlayerIndex].Position = Position;
    PlayerList[newPlayerIndex].Speed = Speed;
    PlayerList[newPlayerIndex].Life = MaxLife;
    PlayerList[newPlayerIndex].Kill = 0;
    PlayerList[newPlayerIndex].Death = 0;

    newPlayerIndex++;
}

void SetPlayerPosition(int playerId, float Position[3])
{
    PlayerList[playerId].Position = Position;
}

void PlayerTakeDamage(int PlayerId, float AttackValue, bool* outIsInLife)
{
    PlayerList[PlayerId].Life -= AttackValue;
    outIsInLife = PlayerList[PlayerId].Life < 1;
}