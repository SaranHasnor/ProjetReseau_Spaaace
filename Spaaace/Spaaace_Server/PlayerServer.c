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
    SpacePlayer_t *newPlayer;
    SpacePlayer_t* tmpPlayer;

    for (uint i = 0; i < ClientPlayerList.size; i++)
    {
        tmpPlayer = (SpacePlayer_t*)ClientPlayerList.content[i];

        string_initStr(&message, "New Player:");
        string_appendStr(&message, strFromFloat(tmpPlayer->Position[0]));
        string_appendStr(&message, ",");
        string_appendStr(&message, strFromFloat(tmpPlayer->Position[1]));
        string_appendStr(&message, ",");
        string_appendStr(&message, strFromFloat(tmpPlayer->Position[2]));
        string_appendStr(&message, ";");
        string_appendStr(&message, strFromInt(tmpPlayer->Kill));
        string_appendStr(&message, ";");
        string_appendStr(&message, strFromInt(tmpPlayer->Death));

        //end of message
        string_appendStr(&message, "!");
        bytestream_init(&stream, message.len);
        bytestream_write(&stream, message.s, message.len);

        SV_sendMessage(senderId, message);
    }



    string_initStr(&message, "New Player:0,0,0;0;0!");
    bytestream_init(&stream, message.len);
    bytestream_write(&stream, message.s, message.len);

    newPlayer = CreatePlayer(defaultposition, 0, 0);

    list_add(&ClientPlayerList, newPlayer);

    SV_sendMessage(-1, message);
}
