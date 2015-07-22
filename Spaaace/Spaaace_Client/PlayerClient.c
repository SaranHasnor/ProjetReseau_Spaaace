#include "PlayerClient.h"

#include <engine_render.h>
#include <utils_matrix.h>

mesh_t* playerMesh;

void CreatePlayerMesh()
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
}

void RenderPlayer(SpacePlayer_t* player, float viewMatrix[16])
{
    vectorCopy(playerMesh->origin, player->Position);
    mat_rotation(playerMesh->rotation,
        player->Angles[0],
        player->Angles[1],
        player->Angles[2]);
    renderMesh(playerMesh,viewMatrix);
}

SpacePlayer_t* CreateNewPlayer()
{
    SpacePlayer_t *player = (SpacePlayer_t*)mem_alloc(sizeof(SpacePlayer_t));

    return player;
}

void ChangeMyPlayerInput(InputValue value, bool buttonState)
{
    switch (value)
    {
        case UpButton:
            myPlayer->input.UpButton = buttonState;
            break;
        case DownButton:
            myPlayer->input.DownButton = buttonState;
            break;
        case LeftButton:
            myPlayer->input.LeftButton = buttonState;
            break;
        case RightButton:
            myPlayer->input.RightButton = buttonState;
            break;
        case UpperButton:
            myPlayer->input.UpperButton = buttonState;
            break;
        case DownerButton:
            myPlayer->input.DownerButton = buttonState;
            break;
    }
}
