#pragma once

#ifndef _PLAYER_SERVER_DEFINED
#define _PLAYER_SERVER_DEFINED

#include "Player.h"
#include <utils_list.h>
#include <network_server.h>

list_t ClientPlayerList;

void PlayerServer_init();
void CreateNewPlayer(int senderId);

#endif


