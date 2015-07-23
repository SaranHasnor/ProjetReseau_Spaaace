#include "bg_input.h"


void PlayerInput_Serialize(PlayerInput_t playerInput, bytestream* stream)
{
    bytestream_write(stream, (byte*)&playerInput, sizeof(PlayerInput_t));
}

void PlayerInput_Deserialize(bytestream stream, PlayerInput_t* playerInput)
{
    bytestream_read(&stream, (byte*)playerInput, sizeof(PlayerInput_t));
}