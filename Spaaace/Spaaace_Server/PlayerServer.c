#include "PlayerServer.h"

void PlayerServer_init()
{
    list_init(&ClientPlayerList);
}

void CreateNewPlayer(int senderId)
{
    float defaultposition[3] = { 0, 0, 0 };
    string message;
    bytestream stream;
	ServerPlayer_t* tmpPlayer;
    ServerPlayer_t *serverPlayer = (ServerPlayer_t*)mem_alloc(sizeof(ServerPlayer_t));

    for (uint i = 0; i < ClientPlayerList.size; i++)
    {
		tmpPlayer = (ServerPlayer_t*)ClientPlayerList.content[i];

        string_initStr(&message, "New Player:");
        string_appendStr(&message, strFromFloat(tmpPlayer->BasePlayer->Position[0]));
        string_appendStr(&message, ",");
		string_appendStr(&message, strFromFloat(tmpPlayer->BasePlayer->Position[1]));
        string_appendStr(&message, ",");
		string_appendStr(&message, strFromFloat(tmpPlayer->BasePlayer->Position[2]));
        string_appendStr(&message, ";");
		string_appendStr(&message, strFromInt(tmpPlayer->BasePlayer->Kill));
        string_appendStr(&message, ";");
		string_appendStr(&message, strFromInt(tmpPlayer->BasePlayer->Death));

        //end of message
        string_appendStr(&message, "!");
        bytestream_init(&stream, message.len);
        bytestream_write(&stream, message.s, message.len);

        SV_sendMessage(senderId, message);
    }



    string_initStr(&message, "New Player:0,0,0;0;0!");
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    serverPlayer->BasePlayer = CreatePlayer(defaultposition, 0, 0);

	serverPlayer->NetworkId = senderId;

    list_add(&ClientPlayerList, serverPlayer);

    SV_sendMessage(-1, message);
}

void GetPlayerWithId(int playerId, ServerPlayer_t* outSpacePlayer)
{
    ServerPlayer_t *player = (ServerPlayer_t*)mem_alloc(sizeof(ServerPlayer_t));
    outSpacePlayer = NULL;
    for (int i = 0; i < ClientPlayerList.size; i++)
    {
        player = ((ServerPlayer_t*)(ClientPlayerList.content[i]));
        if (player->NetworkId == playerId)
        {
            outSpacePlayer = player;
            break;
        }
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
    str_substring(tmp, ',', &str_positionY);
    str_substringIndex(str_position, str_positionX.len + str_positionY.len + 2, str_position.len, &str_positionZ);

    position[0] = parseFloat(str_positionX.s);
    position[1] = parseFloat(str_positionY.s);
    position[2] = parseFloat(str_positionZ.s);
}

void ChangePlayerPosition(int senderId, string message)
{
    float position[3];
    string sendMessage;
    bytestream stream;
    string str_position;
    ServerPlayer_t spacePlayer;

    string_initStr(&str_position, "");

    str_substring(message, ';', &str_position);

    ExtractPosition(str_position, position);

    GetPlayerWithId(senderId, &spacePlayer);

    SetPlayerPosition(&spacePlayer, position);

    string_initStr(&sendMessage, "PlayerPosition:");
    string_appendStr(&sendMessage, strFromFloat(position[0]));
    string_appendStr(&sendMessage, ",");
    string_appendStr(&sendMessage, strFromFloat(position[1]));
    string_appendStr(&sendMessage, ",");
    string_appendStr(&sendMessage, strFromFloat(position[2]));
    string_appendStr(&sendMessage, ";");
    string_appendStr(&sendMessage, strFromInt(senderId));
    string_appendStr(&sendMessage, "!");

    SV_sendMessage(-1, sendMessage);
}
