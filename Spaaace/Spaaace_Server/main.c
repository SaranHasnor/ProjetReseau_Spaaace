#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <network_server.h>
#include <string.h>
#include "PlayerServer.h"

void MessageListener(networkUpdate_t update)
{

    string strMessageByte;
    string strMessage;
    string headerMessage;
    string messageContent;

    string_initStr(&strMessage, "");
    string_initStr(&headerMessage, "");
    string_initStr(&messageContent, "");

    for (uint i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);

        char buffer[128]; // taille max du message
        bytestream_read(&update.messages[i].content, buffer, update.messages[i].content.len);
        string_initStr(&strMessageByte, buffer);
        str_substring(strMessageByte, '!', &strMessage);
        str_substring(strMessage, ':', &headerMessage);
        if (strcmp(headerMessage.s, "Player Connect") == 0)
        {
            CreateNewPlayer(update.messages[i].senderID);
        }
    }
}

int main(int argc, char **argv)
{
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
        if (update.count > 0)
        {
           MessageListener(update);
        }   
    }

    SV_closeServer(&status);

	return 0;
}