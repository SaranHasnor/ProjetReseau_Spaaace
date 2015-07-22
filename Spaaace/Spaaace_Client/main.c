#include <stdio.h>
#include <engine.h>
#include <engine_interface.h>
//#include <engine_render.h>
#include <engine_utils.h>
#include <utils_matrix.h>
#include <game.h>
#include "PlayerClient.h"
#include "client_projectile.h"
#include <utils_time.h>

#include "networkStruct.h"

#include <GL/glut.h>

#include <network_client.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

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

}

void mouseUpFunc(ushort button, int x, int y)
{

}


void updateCamera(inputStruct_t input)
{
    float cameraPosition[3];
	float velocity[3] = {0,0,0};
    float cameraAngle[3];

	if (!myPlayer)
	{
		return;
	}

	if (input.mouseButtons & INPUT_MOUSELEFT)
	{
		float angle[3];

		angle[0] = (float)input.mouseDelta[1];
		angle[1] = -(float)input.mouseDelta[0];
		angle[2] = 0;
		engine_rotateCamera(angle);
        engine_getCameraAngles(cameraAngle);
        //mat_rotation(testMesh->rotation, cameraAngle[0] * (M_PI / 360), cameraAngle[1] * (M_PI / 360), cameraAngle[2] * (M_PI / 360));
	}

	if (input.key_timeHeld('z') > 0)
	{
        velocity[1] += 1.0f;
        ChangeMyPlayerInput(UpButton, true);
    }
    else
    {
        ChangeMyPlayerInput(UpButton, false);
    }

	if (input.key_timeHeld('s') > 0)
	{
        velocity[1] -= 1.0f;
        ChangeMyPlayerInput(DownButton, true);
	}
    else
    {
        ChangeMyPlayerInput(DownButton, false);
    }

	if (input.key_timeHeld('d') > 0)
	{
        velocity[0] += 1.0f;
        ChangeMyPlayerInput(RightButton, true);
	}
    else
    {
        ChangeMyPlayerInput(RightButton, false);
    }

	if (input.key_timeHeld('q') > 0)
	{
        velocity[0] -= 1.0f;
        ChangeMyPlayerInput(LeftButton, true);
	}
    else
    {
        ChangeMyPlayerInput(LeftButton, false);
    }

	if (input.key_timeHeld(' ') > 0)
	{
        velocity[2] += 1.0f;
        ChangeMyPlayerInput(UpperButton, true);
	}
    else
    {
        ChangeMyPlayerInput(UpperButton, false);
    }

	if (input.key_timeHeld('x') > 0)
	{
        velocity[2] -= 1.0f;
        ChangeMyPlayerInput(DownerButton, true);
	}
    else
    {
        ChangeMyPlayerInput(DownerButton, false);
    }

	engine_setCameraVelocity(velocity);
    engine_getCameraPosition(cameraPosition);
}

char* myRandomString = NULL;

void initEngine()
{
	uint i;
    bytestream stream;
    networkStatus_t status;

    list_init(&game.players);
    list_init(&game.projectiles);

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

    CL_connectToServer("127.0.0.1", 4657, stream, SOCKET_PROTOCOL_TCP, &status);
    if (status.error != NETWORK_ERROR_NONE)
        printError(status);
}

void MessageListener(networkUpdate_t update)
{
    for (uint i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);
        
        if (update.messages[i].type == NETWORK_MESSAGE_CONNECT)
        {
			SpacePlayer_t *player = CreateNewPlayer();
			list_add(&game.players, player);

			if (!myPlayer && myRandomString && !strcmp(myRandomString, update.messages[i].content.data))
			{ // That's me!
				myPlayer = player;
			}
        }
		else if (update.messages[i].type == NETWORK_MESSAGE_CUSTOM)
        { // Update
			networkStruct_t net;
			SpacePlayer_t *player = GetPlayerWithId(i);

            if (!player)
            {
                player = CreateNewPlayer();
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
		else if (update.messages[i].type == NETWORK_MESSAGE_EXIT)
		{
			SpacePlayer_t *player = GetPlayerWithId(i);
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
	float viewMatrix[16];

	drawAxis();

	engine_getViewMatrix(viewMatrix);

	drawStars();

    for (uint i = 0; i < game.players.size; i++)
    {
        RenderPlayer(game.players.content[i],viewMatrix);
    }
	//renderMesh(testMesh, viewMatrix);
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
