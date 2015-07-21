#include "PlayerClient.h"

void InitializePlayerClient()
{
    InitializePlayerList();
}

void CreateNewPlayer(bool isMine)
{
    face_t *tempFace;
    ClientPlayer_t newPlayer;

    networkStatus_t status;
    bytestream stream;
    string message;
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
    CreatePlayer(0, 0, 0, isMine, newPlayer.PlayerId);
 
    string_initStr(&message, "PlayerId : ");
    string_initInt(&strPlayerId, newPlayer.PlayerId);
    string_appendStr(&message, &strPlayerId.s);
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    CL_connectToServer("127.0.0.1", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);

    if (!CL_connected)
        printf("Not connected noob !");
}

void PlayerWantToMoveTo(float posX, float posY, float posZ)
{
    string message;
    string floatToCharTmp;

    string_initStr(&message, "PlayerPosition : ");
    string_initFloat(&floatToCharTmp, posX);
    string_appendStr(&message, floatToCharTmp.s);
    string_appendStr(&message, "!");
    string_initFloat(&floatToCharTmp, posY);
    string_appendStr(&message, floatToCharTmp.s);
    string_appendStr(&message, "!");
    string_initFloat(&floatToCharTmp, posZ);
    string_appendStr(&message, floatToCharTmp.s);
    string_appendStr(&message, "!");

    CL_sendMessage(-1, message);
}