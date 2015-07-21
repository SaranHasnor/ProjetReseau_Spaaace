#include "PlayerClient.h"

void PlayerClient_init()
{
    string message;
    bytestream stream;
    networkStatus_t status;

    list_init(&ClientPlayerList);

    string_initStr(&message, "New Player:0,0,0;0;0!");
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    CL_connectToServer("127.0.0.1", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);
}

void RenderClient(float viewMatrix[16])
{
    ClientPlayer_t* player;
    for (int i = 0; i < ClientPlayerList.size; i++)
    {
        player = ((ClientPlayer_t*)ClientPlayerList.content[i]);
        renderMesh(player->PlayerMesh, viewMatrix);
    }
}

void ExtractPosition(string str_position, float position[3])
{
    string str_positionX;
    string str_positionY;
    string str_positionZ;
    string tmp;

    string_initStr(&tmp, "");
    string_initStr(&str_positionX, "");
    string_initStr(&str_positionY, "");
    string_initStr(&str_positionZ, "");

    str_substring(str_position, ',', &str_positionX);
    str_substringIndex(str_position, str_positionX.len + 1, str_position.len, &tmp);
    str_substring(tmp,',', &str_positionY);
    str_substringIndex(str_position, str_positionX.len + str_positionY.len + 2, str_position.len, &str_positionZ);

    position[0] = parseFloat(str_positionX.s);
    position[1] = parseFloat(str_positionY.s);
    position[2] = parseFloat(str_positionZ.s);
}

void CreateNewPlayerStringMessage(string message)
{
    string str_position;
    string str_kill;
    string str_death;
    string tmpMessage;

    string_initStr(&tmpMessage, "");
    string_initStr(&str_position, "");
    string_initStr(&str_kill, "");
    string_initStr(&str_death, "");

    float position[3];
    int kill;
    int death;

    str_substring(message, ';', &str_position);
    str_substringIndex(message, str_position.len+1, message.len, &tmpMessage);
    str_substring(tmpMessage, ';', &str_kill);
    str_substringIndex(message, str_position.len + str_kill.len + 2, message.len, &str_death);

    ExtractPosition(str_position, position);

    kill = parseInt(str_kill.s);
    death = parseInt(str_death.s);

    CreateNewPlayer(position, kill, death);
}

void CreateNewPlayer(float position[3], int kill, int death)
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

    vectorCopy(newPlayer.PlayerMesh->origin, position);

    CreatePlayer(position, kill, death, &newPlayer.Player);

    list_add(&ClientPlayerList, &newPlayer);
}

void GetPlayerWithId(int playerId, ClientPlayer_t* outSpacePlayer)
{
    ClientPlayer_t* player = NULL;
    outSpacePlayer = NULL;
    for (int i = 0; i < ClientPlayerList.size; i++)
    {
        player = ((ClientPlayer_t*)(ClientPlayerList.content[i]));
        if (player->Player.Id == playerId)
        {
            outSpacePlayer = player;
            break;
        }
    }
}

void PlayerWantToMove(float position[3])
{
    string message;
    string floatToCharTmp;

    string_initStr(&message, "PlayerPosition :");
    string_initFloat(&floatToCharTmp, position[0]);
    string_appendStr(&message, floatToCharTmp.s);
    string_appendStr(&message, ",");
    string_initFloat(&floatToCharTmp, position[1]);
    string_appendStr(&message, floatToCharTmp.s);
    string_appendStr(&message, ",");
    string_initFloat(&floatToCharTmp, position[2]);
    string_appendStr(&message, floatToCharTmp.s);

    CL_sendMessage(-1, message);
}

void MovePlayer(float position[3], int PlayerId)
{
    ClientPlayer_t player;
    GetPlayerWithId(PlayerId, &player);
    if (&player != NULL)
    {
        SetPlayerPosition(&player.Player, position);
        vectorCopy(player.PlayerMesh->origin,position);
    }
    else
        printf("Erreur dans la recherche de l'Id demandé.");
}