#include "PlayerServer.h"

ServerPlayer_t* CreateNewPlayer(int senderId)
{
    ServerPlayer_t *serverPlayer = (ServerPlayer_t*)mem_alloc(sizeof(ServerPlayer_t));

	InitPlayer(&serverPlayer->BasePlayer);

	serverPlayer->NetworkId = senderId;

	return serverPlayer;
}
