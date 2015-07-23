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
#define RANGE_STARS 50.0f

float _stars[NB_STARS][3];

void keyDownFunc(uchar key)
{
	if (key == 27) // Escape
	{
		engine_shutdown();
	}
}

void keyUpFunc(uchar key)
{

}

void mouseDownFunc(ushort button, int x, int y)
{
    if (button == INPUT_MOUSELEFT_BUTTON)
        ChangeMyPlayerInput(AttackButton, true);
}

void mouseUpFunc(ushort button, int x, int y)
{
    if (button == INPUT_MOUSELEFT_BUTTON)
        ChangeMyPlayerInput(AttackButton, false);
}


void updateCamera(inputStruct_t input)
{
    if (!myPlayer)
	{
		return;
	}

	if (input.mouseButtons & INPUT_MOUSERIGHT)
	{
		float angle[3];

		angle[0] = (float)input.mouseDelta[1];
		angle[1] = -(float)input.mouseDelta[0];
		angle[2] = 0;
		vectorAdd(myPlayer->Angles, myPlayer->Angles, angle);

		if (myPlayer->Angles[0] < -85)
			myPlayer->Angles[0] = -85;
		if (myPlayer->Angles[0] > 85)
			myPlayer->Angles[0] = 85;

		if (myPlayer->Angles[1] < -180)
			myPlayer->Angles[1] += 360;
		if (myPlayer->Angles[1] > 180)
			myPlayer->Angles[1] -= 360;
	}

	if (input.key_timeHeld('z') > 0)
	{
        ChangeMyPlayerInput(UpButton, true);
    }
    else
    {
        ChangeMyPlayerInput(UpButton, false);
    }

	if (input.key_timeHeld('s') > 0)
	{
        ChangeMyPlayerInput(DownButton, true);
	}
    else
    {
        ChangeMyPlayerInput(DownButton, false);
    }

	if (input.key_timeHeld('d') > 0)
	{
        ChangeMyPlayerInput(RightButton, true);
	}
    else
    {
        ChangeMyPlayerInput(RightButton, false);
    }

	if (input.key_timeHeld('q') > 0)
	{
        ChangeMyPlayerInput(LeftButton, true);
	}
    else
    {
        ChangeMyPlayerInput(LeftButton, false);
    }

	if (input.key_timeHeld(' ') > 0)
	{
        ChangeMyPlayerInput(UpperButton, true);
	}
    else
    {
        ChangeMyPlayerInput(UpperButton, false);
    }

	if (input.key_timeHeld('x') > 0)
	{
        ChangeMyPlayerInput(DownerButton, true);
	}
    else
    {
        ChangeMyPlayerInput(DownerButton, false);
    }

	engine_setCameraPosition(myPlayer->Position);
	engine_setCameraAngles(myPlayer->Angles);
}

char* myRandomString = NULL;

void initEngine()
{
	uint i;
    bytestream stream;
    networkStatus_t status;

    list_init(&game.players);
    list_init(&game.projectiles);

	//UI
	/*interface_pushBlock(relativePlacement(0.48f, 0.48f, 0.04f, 0.04f));
	interface_staticLabel("--", relativePlacement(0, 1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("--", relativePlacement(0, -1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("+", relativePlacement(0, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(1, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(-1, 0, 1, 1), ANCHOR_CENTER);
	interface_popBlock();
	interface_updateLayout();*/

	setupNetwork();

    createProjectileMesh();
    CreatePlayerMesh();

	for (i = 0; i < NB_STARS; i++)
	{
        vectorSet(_stars[i], randomValueBetween(-RANGE_STARS, RANGE_STARS), randomValueBetween(-RANGE_STARS, RANGE_STARS), randomValueBetween(-RANGE_STARS, RANGE_STARS));
		vectorNormalize(_stars[i]);
		vectorScale(_stars[i], 100.0f, _stars[i]);
	}

    bytestream_init(&stream, 16);
	myRandomString = (char*)mem_alloc(sizeof(char)* 16);
	for (i = 0; i < 16; i++)
	{
		myRandomString[i] = (char)randomIntBetween(0, 255);
	}
	bytestream_write(&stream, myRandomString, sizeof(char) * 16);

    CL_connectToServer("10.33.2.72", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);
}

void MessageListener(networkUpdate_t update)
{
	uint i;
    for (i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);
        
        if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
        {
			SpacePlayer_t *player = CreateNewPlayer(update.messages[i].senderID);
			list_add(&game.players, player);

			if (!myPlayer && myRandomString && !strcmp(myRandomString, update.messages[i].content.data))
			{ // That's me!
				myPlayer = player;
			}
        }
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
        { // Update
			networkStruct_t net;
			SpacePlayer_t *player = GetPlayerWithId(update.messages[i].senderID);

			if (player != myPlayer)
			{
				if (!player)
				{
					player = CreateNewPlayer(update.messages[i].senderID);
					player->Id = i;
					list_add(&game.players, player);
				}

				deserializeNetworkStruct(&update.messages[i].content, &net);

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
			SpacePlayer_t *player = GetPlayerWithId(update.messages[i].senderID);
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

    updateGame(time.deltaTimeSeconds);

    if (CL_connected())
    {
        CL_update(&update);
        if (update.count > 0)
        {
            MessageListener(update);
        }
        if (myPlayer != NULL)
        {
            initNetworkStructWithPlayer(&netStruct, myPlayer);
            serializeNetworkStruct(&netStruct, &stream);

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

	drawAxis();

	engine_getViewMatrix(viewMatrix);

	drawStars();

    for (i = 0; i < game.players.size; i++)
    {
        RenderPlayer((SpacePlayer_t*)game.players.content[i],viewMatrix);
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

	listener.keyDownFunc = keyDownFunc;
	listener.keyUpFunc = keyUpFunc;
	listener.mouseDownFunc = mouseDownFunc;
	listener.mouseUpFunc = mouseUpFunc;
	listener.renderFunc = renderFunc;
	listener.updateFunc = updateFunc;

	engine_run(argc, argv, 1200, 600, "Spaaace", listener);

	return 0;
}
