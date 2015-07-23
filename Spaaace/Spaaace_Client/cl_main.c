#include <stdio.h>

#include <utils_matrix.h>
#include <utils_time.h>

#include <engine.h>
#include <engine_interface.h>
#include <engine_utils.h>
#include <GL/glut.h>

#include <network_client.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#include <bg_game.h>
#include <bg_network.h>

#include "cl_player.h"
#include "cl_projectile.h"

#define NB_STARS 1024

player_t *myPlayer = NULL;

float _stars[NB_STARS][3];

void keyDownFunc(uchar key)
{
	if (key == 27) // Escape
	{
		engine_shutdown();
	}
	else if (myPlayer)
	{
		if (key == 'z')
		{
			myPlayer->input.forward = true;
		}
		else if (key == 's')
		{
			myPlayer->input.back = true;
		}
		else if (key == 'd')
		{
			myPlayer->input.right = true;
		}
		else if (key == 'q')
		{
			myPlayer->input.left = true;
		}
		else if (key == ' ')
		{
			myPlayer->input.up = true;
		}
		else if (key == 'x')
		{
			myPlayer->input.down = true;
		}
	}
}

void keyUpFunc(uchar key)
{
	if (myPlayer)
	{
		if (key == 'z')
		{
			myPlayer->input.forward = false;
		}
		else if (key == 's')
		{
			myPlayer->input.back = false;
		}
		else if (key == 'd')
		{
			myPlayer->input.right = false;
		}
		else if (key == 'q')
		{
			myPlayer->input.left = false;
		}
		else if (key == ' ')
		{
			myPlayer->input.up = false;
		}
		else if (key == 'x')
		{
			myPlayer->input.down = false;
		}
	}
}

void mouseDownFunc(ushort button, int x, int y)
{
	if (myPlayer)
	{
		if (button == INPUT_MOUSELEFT_BUTTON)
		{
			myPlayer->input.attack = true;
		}
	}
}

void mouseUpFunc(ushort button, int x, int y)
{
	if (myPlayer)
	{
		if (button == INPUT_MOUSELEFT_BUTTON)
		{
			myPlayer->input.attack = false;
		}
	}
}

void updateCamera(inputStruct_t input)
{
    if (!myPlayer)
	{
		return;
	}

	myPlayer->input.angleDelta[0] = 0;
	myPlayer->input.angleDelta[1] = 0;

	if (input.mouseButtons & INPUT_MOUSERIGHT)
	{
		myPlayer->input.angleDelta[0] = input.mouseDelta[1];
		myPlayer->input.angleDelta[1] = -input.mouseDelta[0];
	}

	engine_setCameraPosition(myPlayer->pos);
	engine_setCameraAngles(myPlayer->ang);
}

void initEngine()
{
	uint i;
    bytestream stream;
    networkStatus_t status;

    list_init(&game.players);
    list_init(&game.projectiles);

	//UI
	interface_pushBlock(relativePlacement(0.48f, 0.48f, 0.04f, 0.04f));
	interface_staticLabel("--", relativePlacement(0, 1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("--", relativePlacement(0, -1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("+", relativePlacement(0, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(1, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(-1, 0, 1, 1), ANCHOR_CENTER);
	interface_popBlock();
	interface_updateLayout();

    createProjectileMesh();
    createPlayerMesh();

	for (i = 0; i < NB_STARS; i++)
	{
        vectorSet(_stars[i], randomValueBetween(-1.0f, 1.0f), randomValueBetween(-1.0f, 1.0f), randomValueBetween(-1.0f, 1.0f));
		vectorNormalize(_stars[i]);
		vectorScale(_stars[i], 100.0f, _stars[i]);
	}

    bytestream_init(&stream, 0);
    CL_connectToServer("127.0.0.1", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);
}

void handleMessages(networkUpdate_t update)
{
	uint i;
    for (i = 0; i < update.count; i++)
    {
#ifdef _DEBUG
        printMessage(update.messages[i]);
#endif
        
        if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
        {
			player_t *player = createNewPlayer(update.messages[i].senderID);
			list_add(&game.players, player);

			if (!myPlayer)
			{ // That's me! Hopefully...
				myPlayer = player;
			}
        }
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
        { // Update
			networkStruct_t net;
			player_t *player = BG_getPlayerWithID(update.messages[i].senderID);

			BG_deserializeNetworkStruct(&update.messages[i].content, &net);

			if (!player)
			{
				if (!net.inputOnly)
				{ // Probably the server, fetch the ID from the struct
					player = BG_getPlayerWithID(net.content.player.id);
					if (!player)
					{
						player = createNewPlayer(net.content.player.id);
						list_add(&game.players, player);
					}
				}
				else
				{
					player = createNewPlayer(update.messages[i].senderID);
					list_add(&game.players, player);
				}
			}

			if (player != myPlayer)
			{ // TODO: If it's us, make this change smoother
				if (net.inputOnly)
				{
					player->input = net.content.input;
				}
				else
				{
					*player = net.content.player;
				}
			}
        }
		else if (update.messages[i].type == NETWORK_MESSAGE_EXIT)
		{
			player_t *player = BG_getPlayerWithID(update.messages[i].senderID);
			list_remove(&game.players, player);
			mem_free(player);
		}
    }
}

void updateFunc(timeStruct_t time, inputStruct_t input)
{
    networkStruct_t netStruct;
    networkUpdate_t update;
    bytestream stream;

	if (time.deltaTime == time.currentTime)
	{
		initEngine();
		time_sync(time.currentTime);
	}

    updateCamera(input);

    BG_gameLoop(time.deltaTimeSeconds);

    if (CL_connected())
    {
        CL_update(&update);
        if (update.count > 0)
        {
            handleMessages(update);
        }
        if (myPlayer != NULL)
        {
            BG_initNetworkStructWithPlayerInput(&netStruct, myPlayer->input);
            BG_serializeNetworkStruct(&netStruct, &stream);

            CL_sendMessage(-1, stream);
        }
    }
}

void drawStars()
{
	uint i;

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for (i = 0; i < NB_STARS; i++)
	{
		glVertex3fv(_stars[i]);
	}
	glEnd();
}

void renderFunc(void)
{
	uint i;
	float viewMatrix[16];

	engine_getViewMatrix(viewMatrix);

	drawStars();

    for (i = 0; i < game.players.size; i++)
    {
        renderPlayer((player_t*)game.players.content[i],viewMatrix);
    }
	
	for (i = 0; i < game.projectiles.size; i++)
	{
		drawProjectile((projectile_t*)game.projectiles.content[i], viewMatrix);
	}
}

int main(int argc, char **argv)
{
	engineListener_t listener;

    time_init();

	setupNetwork(1000, 10000);

	listener.keyDownFunc = keyDownFunc;
	listener.keyUpFunc = keyUpFunc;
	listener.mouseDownFunc = mouseDownFunc;
	listener.mouseUpFunc = mouseUpFunc;
	listener.renderFunc = renderFunc;
	listener.updateFunc = updateFunc;

	engine_run(argc, argv, 1200, 600, "Spaaace", listener);

	return 0;
}
