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

void serializeNetworkStruct(networkStruct_t *netStruct, bytestream *stream)
{
	bytestream_init(stream, sizeof(bool)+netStruct->inputOnly ? sizeof(PlayerInput_t) : sizeof(SpacePlayer_t));
	bytestream_write(stream, &netStruct->inputOnly, sizeof(bool));
	if (netStruct->inputOnly)
	{
		PlayerInput_Serialize(netStruct->content.input, stream);
	}
	else
	{
		Player_Serialize(netStruct->content.player, stream);
	}
}

void deserializeNetworkStruct(bytestream *stream, networkStruct_t *netStruct)
{
	bytestream_read(stream, &netStruct->inputOnly, sizeof(bool));
	if (netStruct->inputOnly)
	{
		PlayerInput_Deserialize(*stream, &netStruct->content.input);
	}
	else
	{
		Player_Deserialize(*stream, &netStruct->content.player);
	}
}