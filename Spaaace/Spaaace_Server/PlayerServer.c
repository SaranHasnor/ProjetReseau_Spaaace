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

    for (int i = 0; i < ClientPlayerList.size; i++)
    {
        tmpPlayer = (SpacePlayer_t*)ClientPlayerList.content[i];

        string_initStr(&message, "New Player:");
        string_appendStr(&message, strFromFloat(tmpPlayer->player.Position[0]));
        string_appendStr(&message, ",");
        string_appendStr(&message, strFromFloat(tmpPlayer->player.Position[1]));
        string_appendStr(&message, ",");
        string_appendStr(&message, strFromFloat(tmpPlayer->player.Position[2]));
        string_appendStr(&message, ";");
        string_appendStr(&message, strFromInt(tmpPlayer->player.Kill));
        string_appendStr(&message, ";");
        string_appendStr(&message, strFromInt(tmpPlayer->player.Death));

        //end of message
        string_appendStr(&message, "!");
        bytestream_init(&stream, message.len);
        bytestream_write(&stream, message.s, message.len);

        SV_sendMessage(senderId, message);
    }



    string_initStr(&message, "New Player:0,0,0;0;0!");
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    CreatePlayer(defaultposition, 0, 0, &serverPlayer->player);

    serverPlayer->NetworkId = senderId;

    list_add(&ClientPlayerList, &serverPlayer);

    SV_sendMessage(-1, message);
}
