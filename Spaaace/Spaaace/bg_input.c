#include "bg_input.h"


void BG_serializePlayerInput(playerInput_t *in, bytestream *out)
{
    bytestream_write(out, (byte*)in, sizeof(playerInput_t));
}

void BG_deserializePlayerInput(bytestream *in, playerInput_t *out)
{
    bytestream_read(in, (byte*)out, sizeof(playerInput_t));
}