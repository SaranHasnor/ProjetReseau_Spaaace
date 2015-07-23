#include "cl_projectile.h"

#include <engine_render.h>
#include <utils_matrix.h>

mesh_t *_projMesh = NULL;

void createProjectileMesh()
{
	uint i;
	_projMesh = newMesh();

	// Head
	addFace();
	addVertex(0.0f, 0.1f, 0.0f, 0.5f, 1.0f);
	addVertex(-0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, 0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, 0.1f, 0.0f, 0.5f, 1.0f);
	addVertex(-0.1f, 0.0f, -0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, 0.1f, 0.0f, 0.5f, 1.0f);
	addVertex(-0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(-0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, 0.1f, 0.0f, 0.5f, 1.0f);
	addVertex(0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	// Tail
	addFace();
	addVertex(0.0f, -0.5f, 0.0f, 0.5f, 0.0f);
	addVertex(-0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, 0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, -0.5f, 0.0f, 0.5f, 0.0f);
	addVertex(-0.1f, 0.0f, -0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, -0.5f, 0.0f, 0.5f, 0.0f);
	addVertex(-0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(-0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	addFace();
	addVertex(0.0f, -0.5f, 0.0f, 0.5f, 0.0f);
	addVertex(0.1f, 0.0f, 0.1f, 0.0f, 0.5f);
	addVertex(0.1f, 0.0f, -0.1f, 1.0f, 0.5f);

	for (i = 0; i < _projMesh->nbFaces; i++)
	{
		vectorSet(_projMesh->faces[i]->color, 1.0f, 0.5f, 0.5f);
	}

    updateMeshGeometry(_projMesh);
}

void drawProjectile(projectile_t *projectile, float viewMatrix[16])
{
	vectorCopy(_projMesh->origin, projectile->pos);
	mat_rotation(_projMesh->rotation,
		-projectile->ang[0],
		projectile->ang[1] - 90,
		projectile->ang[2],
		true);
	
	renderMesh(_projMesh, viewMatrix);
}