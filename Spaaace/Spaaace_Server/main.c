#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <network_server.h>

int main(int argc, char **argv)
{
	//printf(":D\n");
    char* message;
    networkUpdate_t update;
    networkStatus_t status;

    setupNetwork();
    SV_initServer(2, 4657, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);

    while (true)
    {
        SV_checkForNewClients();
        SV_update(&update);
        for (int i = 0; i < update.count; i++)
            printMessage(update.messages[i]);
    }

	return 0;
}