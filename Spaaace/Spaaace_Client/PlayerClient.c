#include "PlayerClient.h"

void CreateNewPlayer(bool isMine)
{
    face_t *tempFace;
    ClientPlayer_t newPlayer;

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

    CreatePlayer(0, 0, 0, isMine, newPlayer.PlayerId);

    networkStatus_t status;
    bytestream stream;
    string message;
    string_initStr(&message, "PlayerId : " + newPlayer.PlayerId);
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    CL_connectToServer("192.168.1.1", 80, stream, SOCKET_PROTOCOL_TCP, &status);
}

void PlayerWantToMoveTo(float posX, float posY, float posZ)
{
    string message;
    string_initStr(&message, "PlayerPosition : " + posX + "!" + posY + "!" + posZ + ";");
    CL_sendMessage(-1, message);
}