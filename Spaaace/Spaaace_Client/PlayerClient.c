#include "PlayerClient.h"

void PlayerClient_init()
{
    string message;
    bytestream stream;
    networkStatus_t status;

    string_initStr(&message, "New Player");
    bytestream_write(&stream, message.s, message.len);

    CL_connectToServer("127.0.0.1", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);
}

void CreateNewPlayer(bool isMine)
{
    face_t *tempFace;
    ClientPlayer_t newPlayer;
    PlayerId_t playerId;

    string strPlayerId;

    newPlayer.PlayerMesh = newMesh();

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

    updateMeshGeometry(newPlayer.PlayerMesh);

    newPlayer.PlayerId = -1;
    CreatePlayer(0, 0, 0, isMine, &playerId);
}

void MovePlayer(float posX, float posY, float posZ)
{

}