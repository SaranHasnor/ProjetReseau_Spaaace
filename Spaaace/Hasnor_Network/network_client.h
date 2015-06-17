#include "network_utils.h"
#include <utils_string.h>
#include <utils_types.h>

extern void setupNetwork();
extern void shutdownNetwork();

void CL_connectToServer(const char *address, unsigned short port, bytestream clientInfo, networkStatus_t *status);
void CL_disconnectFromServer(networkStatus_t *status);

bool CL_connected();

void CL_sendMessage(int targetID, string message);
void CL_update(networkUpdate_t *update);
