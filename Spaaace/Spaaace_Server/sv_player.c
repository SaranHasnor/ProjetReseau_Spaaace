#include "sv_player.h"

ServerPlayer_t* CreateNewPlayer(int senderId)
{
    ServerPlayer_t *serverPlayer = (ServerPlayer_t*)mem_alloc(sizeof(ServerPlayer_t));

	InitPlayer(&serverPlayer->BasePlayer, senderId);

	return serverPlayer;
}
