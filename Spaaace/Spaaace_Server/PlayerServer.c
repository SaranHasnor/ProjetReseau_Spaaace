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
