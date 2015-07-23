#include "cl_player.h"

#include <engine_render.h>
#include <utils_matrix.h>

mesh_t *playerMeshFirstPerson;
mesh_t *playerMeshThirdPerson;

void createPlayerMesh()
{
    face_t *tempFace;
	texture_t *metal = textureFromPath("textures/basemetal1.jpg");
	texture_t *floor = textureFromPath("textures/floor7.jpg");
	texture_t *glass = textureFromPath("textures/glass2.jpg");

    playerMeshFirstPerson = newMesh();

	tempFace = addFace();
	tempFace->texture = metal;
    addVertex(0.5f, 0.0f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, 2.0f, -0.5f, 0.0f, 0.0f);
    addVertex(0.5f, 0.0f, 0.25f, 1.0f, 0.0f);

    tempFace = addFace();
	tempFace->texture = metal;
    addVertex(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f);
    addVertex(-0.5f, 2.0f, -0.5f, 0.0f, 0.0f);
    addVertex(-0.5f, 0.0f, 0.25f, 1.0f, 0.0f);

	tempFace = addFace();
	tempFace->texture = floor;
    addVertex(-0.5f, 2.0f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, 2.0f, -0.5f, 1.0f, 1.0f);
    addVertex(0.5f, -2.0f, -0.5f, 1.0f, 0.0f);

	tempFace = addFace();
    tempFace->texture = floor;
    addVertex(-0.5f, 2.0f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, -2.0f, -0.5f, 1.0f, 0.0f);
    addVertex(-0.5f, -2.0f, -0.5f, 0.0f, 0.0f);

	/*tempFace = addFace();
	tempFace->texture = glass;
	tempFace->color[3] = 0.2f;
    addVertex(-0.5f, 2.0f, -0.5f, 0.0f, 0.0f);
    addVertex(0.5f, 0.0f, 0.25f, 1.0f, 1.0f);
    addVertex(0.5f, 2.0f, -0.5f, 1.0f, 0.0f);

    tempFace = addFace();
	tempFace->texture = glass;
	tempFace->color[3] = 0.2f;
    addVertex(-0.5f, 2.0f, -0.5f, 0.0f, 0.0f);
    addVertex(-0.5f, 0.0f, 0.25f, 0.0f, 1.0f);
    addVertex(0.5f, 0.0f, 0.25f, 1.0f, 1.0f);*/   

    updateMeshGeometry(playerMeshFirstPerson);

	// Third person mesh is much smaller
	playerMeshThirdPerson = newMesh();

    tempFace = addFace();
	tempFace->texture = metal;
    addVertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, 1.0f, -0.5f, 0.0f, 0.0f);
    addVertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f);

    tempFace = addFace();
	tempFace->texture = metal;
    addVertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f);
    addVertex(-0.5f, 1.0f, -0.5f, 0.0f, 0.0f);
    addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f);

	tempFace = addFace();
	tempFace->texture = floor;
    addVertex(-0.5f, 1.0f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, 1.0f, -0.5f, 1.0f, 1.0f);
    addVertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f);

	tempFace = addFace();
    tempFace->texture = floor;
    addVertex(-0.5f, 1.0f, -0.5f, 0.0f, 1.0f);
    addVertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
    addVertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f);

	// Bottom
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	addVertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f);
	addVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	addVertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f);

	// Top
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f);
	addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f);
	addVertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	// Left
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

	// Right
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);
	addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	// Front
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	addVertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f);
	addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
	addVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	addVertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f);

	// Back
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	
	tempFace = addFace();
	tempFace->texture = metal;
	addVertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
	addVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f);
	addVertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f);

    updateMeshGeometry(playerMeshThirdPerson);
}

void renderPlayer(player_t *player, float viewMatrix[16])
{
	if (player == myPlayer)
	{
		vectorCopy(playerMeshFirstPerson->origin, player->pos);
		mat_rotation(playerMeshFirstPerson->rotation,
			-player->ang[0],
			player->ang[1] - 90,
			player->ang[2],
			true);
    
		renderMesh(playerMeshFirstPerson, viewMatrix);
	}
	else
	{
		vectorCopy(playerMeshThirdPerson->origin, player->pos);
		mat_rotation(playerMeshThirdPerson->rotation,
			-player->ang[0],
			player->ang[1] - 90,
			player->ang[2],
			true);
    
		renderMesh(playerMeshThirdPerson, viewMatrix);
	}
}

player_t *createNewPlayer(int id)
{
    player_t *player = (player_t*)mem_alloc(sizeof(player_t));
	BG_initPlayer(player, id);
    return player;
}