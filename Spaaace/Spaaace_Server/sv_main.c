#include <stdio.h>
#include <stdlib.h>
#include <bg_game.h>
#include <network_server.h>
#include <string.h>
#include <Windows.h>
#include "sv_player.h"
#include <bg_network.h>
#include <utils_time.h>

void handleMessages(networkUpdate_t update)
{
	uint i, j;
    for (i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);

		if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
		{
			serverPlayer_t *player = createNewPlayer(update.messages[i].senderID);
			list_add(&game.players, player);
			
			bytestream_init(&player->connectionData, update.messages[i].content.len);
			bytestream_write(&player->connectionData, update.messages[i].content.data, update.messages[i].content.len);
			player->connectionData.cursor = 0;

			for (j = 0; j < game.players.size - 1; j++)
			{ // Sync their player data with the new one
				networkStruct_t net;
				bytestream stream;
				BG_initNetworkStructWithPlayer(&net, ((serverPlayer_t*)game.players.content[j])->BasePlayer);
				BG_serializeNetworkStruct(&net, &stream);
				SV_sendMessage(update.messages[i].senderID, stream);
			}
		}
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
		{ // Update
			networkStruct_t net;
			player_t *player = BG_getPlayerWithID(update.messages[i].senderID);
			BG_deserializeNetworkStruct(&update.messages[i].content, &net);

			if (net.inputOnly)
			{
				player->input = net.content.input;
			}
			else
			{
				*player = net.content.player;
			}
		}
		else if (update.messages[i].type == NETWORK_MESSAGE_EXIT)
		{

		}
    }
}

int main(int argc, char **argv)
{
    networkUpdate_t update;
    networkStatus_t status;
	double lastTime = 0.0;
	double lastBigUpdateTime = 0.0;

    setupNetwork(1000, 10000);
    SV_initServer(32, 4657, SOCKET_PROTOCOL_TCP, &status);

	time_init();

    if (status.error != NETWORK_ERROR_NONE)
        printError(status);

    while (true)
    {
		double newTime = time_current_sec();
		float deltaTime = (float)(newTime - lastTime);
		float timeSinceLastBigUpdate = (float)(newTime - lastBigUpdateTime);

        SV_checkForNewClients();
        SV_update(&update);
        if (update.count > 0)
        {
           handleMessages(update);
        }

		BG_gameLoop(deltaTime);
		lastTime = newTime;

		if (timeSinceLastBigUpdate >= 0.25f)
		{
			uint i;
			for (i = 0; i < game.players.size; i++)
			{
				networkStruct_t netStruct;
				bytestream stream;
				BG_initNetworkStructWithPlayer(&netStruct, *(player_t*)game.players.content[i]);
				BG_serializeNetworkStruct(&netStruct, &stream);
				SV_sendMessage(-1, stream);
			}

			timeSinceLastBigUpdate = (float)newTime;
		}

		Sleep(10);
    }

    SV_closeServer(&status);

	return 0;
}