#include "networkStruct.h"

void initNetworkStructWithPlayer(networkStruct_t *netStruct, SpacePlayer_t *player)
{
	netStruct->inputOnly = false;
	netStruct->content.player = *player;
}

void initNetworkStructWithPlayerInput(networkStruct_t *netStruct, PlayerInput_t input)
{
	netStruct->inputOnly = true;
	netStruct->content.input = input;
}