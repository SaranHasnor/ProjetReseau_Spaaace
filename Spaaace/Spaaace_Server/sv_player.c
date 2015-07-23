#include "sv_player.h"

serverPlayer_t *createNewPlayer(int id)
{
    serverPlayer_t *serverPlayer = (serverPlayer_t*)mem_alloc(sizeof(serverPlayer_t));

	BG_initPlayer(&serverPlayer->BasePlayer, id);

	return serverPlayer;
}
