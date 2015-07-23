#include "bg_network.h"

void BG_initNetworkStructWithPlayer(networkStruct_t *netStruct, player_t player)
{
	netStruct->inputOnly = false;
	netStruct->content.player = player;
}

void BG_initNetworkStructWithPlayerInput(networkStruct_t *netStruct, playerInput_t input)
{
	netStruct->inputOnly = true;
	netStruct->content.input = input;
}

void BG_serializeNetworkStruct(networkStruct_t *in, bytestream *out)
{
	bytestream_init(out, sizeof(bool) + (in->inputOnly ? sizeof(playerInput_t) : sizeof(player_t)));
	bytestream_write(out, (byte*)&in->inputOnly, sizeof(bool));
	if (in->inputOnly)
	{
		BG_serializePlayerInput(&in->content.input, out);
	}
	else
	{
		BG_serializePlayer(&in->content.player, out);
	}
}

void BG_deserializeNetworkStruct(bytestream *in, networkStruct_t *out)
{
	bytestream_read(in, (byte*)&out->inputOnly, sizeof(bool));
	if (out->inputOnly)
	{
		BG_deserializePlayerInput(in, &out->content.input);
	}
	else
	{
		BG_deserializePlayer(in, &out->content.player);
	}
}