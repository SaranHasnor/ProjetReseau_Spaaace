#include "bg_player.h"

typedef union {
	playerInput_t	input;
	player_t	player;
} networkStructContent_u;

typedef struct {
	bool					inputOnly;
	networkStructContent_u	content;
} networkStruct_t;

void BG_initNetworkStructWithPlayer(networkStruct_t *netStruct, player_t player);
void BG_initNetworkStructWithPlayerInput(networkStruct_t *netStruct, playerInput_t input);

void BG_serializeNetworkStruct(networkStruct_t *in, bytestream *out);
void BG_deserializeNetworkStruct(bytestream *in, networkStruct_t *out);