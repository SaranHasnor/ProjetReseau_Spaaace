#include "cl_player.h"

#include <engine_render.h>
#include <utils_matrix.h>

mesh_t *playerMesh;

void createPlayerMesh()
{
    face_t *tempFace;

    playerMesh = newMesh();

    // Each face must be a triangle
    tempFace = addFace();
    tempFace->color[0] = 0.0f;
    addVertex(0.5f, 1.0f, -0.25f, 0.0f, 1.0f);
    addVertex(0.5f, 2.0f, -0.25f, 0.0f, 0.0f);
    addVertex(0.5f, 1.0f, 0.25f, 1.0f, 0.0f);

    tempFace = addFace();
    tempFace->color[1] = 0.0f;
    addVertex(-0.5f, 1.0f, -0.25f, 0.0f, 1.0f);
    addVertex(-0.5f, 2.0f, -0.25f, 0.0f, 0.0f);
    addVertex(-0.5f, 1.0f, 0.25f, 1.0f, 0.0f);

    updateMeshGeometry(playerMesh);
}

void renderPlayer(player_t *player, float viewMatrix[16])
{
    vectorCopy(playerMesh->origin, player->pos);
	mat_rotation(playerMesh->rotation,
		-player->ang[0],
		player->ang[1] - 90,
		player->ang[2],
		true);
    
	renderMesh(playerMesh, viewMatrix);
}

player_t *createNewPlayer(int id)
{
    player_t *player = (player_t*)mem_alloc(sizeof(player_t));
	BG_initPlayer(player, id);
    return player;
}