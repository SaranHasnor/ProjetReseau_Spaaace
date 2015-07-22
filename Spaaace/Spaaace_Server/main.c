#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <network_server.h>
#include <string.h>
#include "PlayerServer.h"

void MessageListener(networkUpdate_t update)
{
    for (uint i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);

		if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
		{
			CreateNewPlayer(update.messages[i].senderID);
		}
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
		{ // Update
			//ChangePlayerPosition(update.messages[i].senderID, messageContent);
		}
    }
}

int main(int argc, char **argv)
{
    networkUpdate_t update;
    networkStatus_t status;

    setupNetwork();
    SV_initServer(32, 4657, SOCKET_PROTOCOL_TCP, &status);

    if (status.error != NETWORK_ERROR_NONE)
        printError(status);

    while (true)
    {
        SV_checkForNewClients();
        SV_update(&update);
        if (update.count > 0)
        {
           MessageListener(update);
        }
    }

    SV_closeServer(&status);

	return 0;
}