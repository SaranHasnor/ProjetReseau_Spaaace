#include "Player.h"

float Speed = 1.0f;
float MaxLife = 100;
unsigned int newPlayerIndex = 1;
unsigned int _MaxPlayer = 2;
SpacePlayer_t MyPlayer;
SpacePlayer_t* PlayerList;

void Initialize()
{
    PlayerList = (SpacePlayer_t*)mem_alloc(sizeof(SpacePlayer_t) * _MaxPlayer);
}

void ChangeMaxLife(float maxLife)
{
    MaxLife = maxLife;
}

void CreatePlayer(float posX, float posY, float posZ, bool isMine, int* outId)
{
    SpacePlayer_t newPlayer;

    newPlayer.Id = newPlayerIndex;
    newPlayer.PositionX = posX;
    newPlayer.PositionY = posY;
    newPlayer.PositionZ = posZ;
    newPlayer.Speed = Speed;
    newPlayer.Life = MaxLife;
    newPlayer.Kill = 0;
    newPlayer.Death = 0;

    if (isMine)
    {
        MyPlayer = newPlayer;
    }

    PlayerList[newPlayerIndex] = newPlayer;

    *outId = newPlayerIndex;

    newPlayerIndex++;
}

void SetPlayerPosition(int playerId, float posX, float posY, float posZ)
{
    if (MyPlayer.Id = playerId)
    {
        MyPlayer.PositionX = posX;
        MyPlayer.PositionY = posY;
        MyPlayer.PositionZ = posZ;
    }
    PlayerList[playerId].PositionX = posX;
    PlayerList[playerId].PositionY = posY;
    PlayerList[playerId].PositionZ = posZ;
}

void PlayerTakeDamage(int PlayerId, float AttackValue, bool* outIsInLife)
{
    PlayerList[PlayerId].Life -= AttackValue;
    outIsInLife = PlayerList[PlayerId].Life < 1;
}