#include "Player.h"

typedef union {
	PlayerInput_t	input;
	SpacePlayer_t	player;
} networkStructContent_u;

typedef struct {
	bool					inputOnly;
	networkStructContent_u	content;
} networkStruct_t;

void initNetworkStructWithPlayer(networkStruct_t *netStruct, SpacePlayer_t *player);
void initNetworkStructWithPlayerInput(networkStruct_t *netStruct, PlayerInput_t input);

void serializeNetworkStruct(networkStruct_t *netStruct, bytestream *stream);
void deserializeNetworkStruct(bytestream *stream, networkStruct_t *netStruct);