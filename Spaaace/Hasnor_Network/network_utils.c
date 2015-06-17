#include <utils.h>
#include "network_utils.h"

void printError(networkStatus_t status)
{
	if (status.error != NETWORK_ERROR_NONE)
	{
		printf("Error: %i\n", status.socketError);
	}
}

void _printConnectionName(int id)
{
	if (id == -1)
	{
		printf("all");
	}
	else if (id == 0)
	{
		printf("server");
	}
	else
	{
		printf("client %i", id);
	}
}

void printMessage(networkMessage_t message)
{
	switch (message.type)
	{
	case NETWORK_MESSAGE_CONNECT:
		printf("CONNECT (");
		break;
	case NETWORK_MESSAGE_HEARTBEAT:
		printf("HEARTBEAT (");
		break;
	case NETWORK_MESSAGE_EXIT:
		printf("EXIT (");
		break;
	case NETWORK_MESSAGE_CUSTOM:
	default:
		printf("MESSAGE (");
		break;
	}

	_printConnectionName(message.senderID);

	printf(" -> ");

	_printConnectionName(message.receiverID);

	if (message.content.len > 0)
	{
		printf("): %.*s\n", message.content.len, message.content.data);
	}
	else
	{
		printf(")\n");
	}
}