#pragma once

#ifndef _PLAYER_CLIENT_DEFINED
#define _PLAYER_CLIENT_DEFINED

#include "Player.h"
#include <string.h>
#include <engine_render.h>
#include <network_client.h>

typedef struct {
    int PlayerId;
    mesh_t* PlayerMesh;
} ClientPlayer_t;



#endif
