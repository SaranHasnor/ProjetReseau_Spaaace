#ifndef _NETWORK_TOOLS_DEFINED
#define _NETWORK_TOOLS_DEFINED

#include <utils.h>
#include "network_utils.h"

typedef enum {
	NETWORK_MODE_LOCAL,
	NETWORK_MODE_CLIENT,
	NETWORK_MODE_HOST,
	NETWORK_MODE_MASTER
} networkMode_t;

typedef enum {
	SOCKET_TYPE_TCP,
	SOCKET_TYPE_UDP
} socketType_t;

networkMode_t currentNetworkMode();
uint maxConnections();

void setupNetwork();
void shutdownNetwork();

bool createHostSocket(int maxConnections, unsigned short port, socketType_t type, networkStatus_t *status);
int createSocket(const char *address, unsigned short port, socketType_t type, networkStatus_t *status);

bool tryToConnect(bytestream clientInfo, networkStatus_t *status);
bool getNewClient();

void dropClient(int id);
void disconnect();

void checkForTimeOuts();

void sendMessage(networkMessageType_t type, int senderID, int receiverID, bytestream content);
void receiveMessages(networkUpdate_t *update);

#endif