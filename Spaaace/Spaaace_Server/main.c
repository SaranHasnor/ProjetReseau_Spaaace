#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <network_server.h>
#include <string.h>
#include <Windows.h>
#include "PlayerServer.h"
#include "networkStruct.h"
#include <utils_time.h>

void MessageListener(networkUpdate_t update)
{
    for (uint i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);

		if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
		{
			ServerPlayer_t *player = CreateNewPlayer(update.messages[i].senderID);
			list_add(&game.players, player);
			
			bytestream_init(&player->connectionData, update.messages[i].content.len);
			bytestream_write(&player->connectionData, update.messages[i].content.data, update.messages[i].content.len);
			player->connectionData.cursor = 0;

			for (uint i = 0; i < game.players.size - 1; i++)
			{ // Sync their player data with the new one
				networkStruct_t net;
				bytestream stream;
				initNetworkStructWithPlayer(&net, game.players.content[i]);
				serializeNetworkStruct(&net, &stream);
				SV_sendMessage(update.messages[i].senderID, stream);
			}
		}
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
		{ // Update
			networkStruct_t net;
			SpacePlayer_t *player = GetPlayerWithId(update.messages[i].senderID);
			deserializeNetworkStruct(&update.messages[i].content, &net);

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

    setupNetwork();
    SV_initServer(32, 4657, SOCKET_PROTOCOL_TCP, &status);

	time_init();

    if (status.error != NETWORK_ERROR_NONE)
        printError(status);

    while (true)
    {
		double newTime = time_current_sec();
		float deltaTime = (float)(newTime - lastTime);

        SV_checkForNewClients();
        SV_update(&update);
        if (update.count > 0)
        {
           MessageListener(update);
        }

		updateGame(deltaTime);

		Sleep(10);
    }

    SV_closeServer(&status);

	return 0;
}