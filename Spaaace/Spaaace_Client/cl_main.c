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

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions (sprintf...)

#define NB_STARS 1024

player_t *myPlayer = NULL;
char *healthString = NULL;

mesh_t *_skybox = NULL;
float _stars[NB_STARS][3];

void keyDownFunc(uchar key)
{
	if (key == 27) // Escape
	{
		CL_disconnectFromServer(NULL);
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

void createSkybox()
{
	const float boxSize = 150.0f;
	uint i; 

	_skybox = newMesh();

	// Bottom
	addFace();
	addVertex(-boxSize, boxSize, -boxSize, 1.0f, 1.0f);
	addVertex(-boxSize, -boxSize, -boxSize, 1.0f, 0.0f);
	addVertex(boxSize, -boxSize, -boxSize, 0.0f, 0.0f);
	addFace();
	addVertex(-boxSize, boxSize, -boxSize, 1.0f, 1.0f);
	addVertex(boxSize, -boxSize, -boxSize, 0.0f, 0.0f);
	addVertex(boxSize, boxSize, -boxSize, 0.0f, 1.0f);

	// Top
	addFace();
	addVertex(-boxSize, boxSize, boxSize, 0.0f, 1.0f);
	addVertex(-boxSize, -boxSize, boxSize, 0.0f, 0.0f);
	addVertex(boxSize, -boxSize, boxSize, 1.0f, 0.0f);
	addFace();
	addVertex(-boxSize, boxSize, boxSize, 0.0f, 1.0f);
	addVertex(boxSize, -boxSize, boxSize, 1.0f, 0.0f);
	addVertex(boxSize, boxSize, boxSize, 1.0f, 1.0f);

	// Left
	addFace();
	addVertex(-boxSize, -boxSize, boxSize, 0.0f, 1.0f);
	addVertex(-boxSize, -boxSize, -boxSize, 0.0f, 0.0f);
	addVertex(-boxSize, boxSize, -boxSize, 1.0f, 0.0f);
	addFace();
	addVertex(-boxSize, -boxSize, boxSize, 0.0f, 1.0f);
	addVertex(-boxSize, boxSize, -boxSize, 1.0f, 0.0f);
	addVertex(-boxSize, boxSize, boxSize, 1.0f, 1.0f);

	// Right
	addFace();
	addVertex(boxSize, -boxSize, boxSize, 1.0f, 1.0f);
	addVertex(boxSize, -boxSize, -boxSize, 1.0f, 0.0f);
	addVertex(boxSize, boxSize, -boxSize, 0.0f, 0.0f);
	addFace();
	addVertex(boxSize, -boxSize, boxSize, 1.0f, 1.0f);
	addVertex(boxSize, boxSize, -boxSize, 0.0f, 0.0f);
	addVertex(boxSize, boxSize, boxSize, 0.0f, 1.0f);

	// Front
	addFace();
	addVertex(-boxSize, boxSize, boxSize, 1.0f, 1.0f);
	addVertex(-boxSize, boxSize, -boxSize, 1.0f, 0.0f);
	addVertex(boxSize, boxSize, -boxSize, 0.0f, 0.0f);
	addFace();
	addVertex(-boxSize, boxSize, boxSize, 1.0f, 1.0f);
	addVertex(boxSize, boxSize, -boxSize, 0.0f, 0.0f);
	addVertex(boxSize, boxSize, boxSize, 0.0f, 1.0f);

	// Back
	addFace();
	addVertex(-boxSize, -boxSize, boxSize, 0.0f, 1.0f);
	addVertex(-boxSize, -boxSize, -boxSize, 0.0f, 0.0f);
	addVertex(boxSize, -boxSize, -boxSize, 1.0f, 0.0f);
	addFace();
	addVertex(-boxSize, -boxSize, boxSize, 0.0f, 1.0f);
	addVertex(boxSize, -boxSize, -boxSize, 1.0f, 0.0f);
	addVertex(boxSize, -boxSize, boxSize, 1.0f, 1.0f);

	for (i = 0; i < _skybox->nbFaces; i++)
	{
		vectorCopy(_skybox->faces[i]->color, nullVec);
	}
	
	updateMeshGeometry(_skybox);
}

void showLoginInterface();
void disconnectFromServer(void)
{
	uint i;
	CL_disconnectFromServer(NULL);
	
	for (i = 0; i < game.players.size; i++)
	{ // Clunkiest loop I've ever made, but it does the job
		mem_free(game.players.content[i]);
		list_removeAt(&game.players, i--);
	}

	for (i = 0; i < game.projectiles.size; i++)
	{
		mem_free(game.projectiles.content[i]);
		list_removeAt(&game.projectiles, i--);
	}

	myPlayer = NULL;

	showLoginInterface();
}

void showGameplayInterface()
{
	interface_deleteAllObjects();

	interface_pushBlock(relativePlacement(0.48f, 0.48f, 0.04f, 0.04f));
	interface_staticLabel("--", relativePlacement(0, 1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("--", relativePlacement(0, -1, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("+", relativePlacement(0, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(1, 0, 1, 1), ANCHOR_CENTER);
	interface_staticLabel("|", relativePlacement(-1, 0, 1, 1), ANCHOR_CENTER);
	interface_popBlock();

	interface_dynamicLabel(&healthString, relativePlacement(0.0f, 0.8f, 1.0f, 0.2f), ANCHOR_CENTER);

	interface_staticButton("Deconnexion", relativePlacement(0.8f, 0.0f, 0.2f, 0.1f), disconnectFromServer);

	interface_updateLayout();
}

void initEngine()
{
	uint i;

	healthString = (char*)mem_alloc(sizeof(char) * 64);

	showGameplayInterface();

	createSkybox();
    createProjectileMesh();
    createPlayerMesh();

	for (i = 0; i < NB_STARS; i++)
	{
        vectorSet(_stars[i], randomValueBetween(-1.0f, 1.0f), randomValueBetween(-1.0f, 1.0f), randomValueBetween(-1.0f, 1.0f));
		vectorNormalize(_stars[i]);
		vectorScale(_stars[i], 100.0f, _stars[i]);
	}
}

char *address = NULL;
int *port = NULL;
void connect(void)
{
	networkStatus_t status;
	bytestream clientInfo;
	bytestream_init(&clientInfo, 0);

	CL_connectToServer(address, (unsigned short)*port, clientInfo, SOCKET_PROTOCOL_TCP, &status);

	bytestream_destroy(&clientInfo);

	if (status.error == NETWORK_ERROR_NONE)
	{ // Success
		initEngine();
	}
	else
	{
		printError(status);
	}
}

void showLoginInterface()
{
	uint curObj;

	interface_deleteAllObjects();

	if (!address)
	{
		address = (char*)mem_alloc(sizeof(char) * 16);
		strcpy(address, "127.0.0.1");
	}
	if (!port)
	{
		port = (int*)mem_alloc(sizeof(int));
		*port = 5875;
	}

	interface_pushBlock(relativePlacement(0.25f, 0.25f, 0.5f, 0.5f));

	curObj = interface_textField(relativePlacement(0.0f, 0.2f, 1.0f, 0.2f), FIELDTYPE_TEXT);
	interface_setTextFieldValue(curObj, 0.0f, 15.0f, (void**)&address, true);
	curObj = interface_textField(relativePlacement(0.0f, 0.6f, 1.0f, 0.2f), FIELDTYPE_INT);
	interface_setTextFieldValue(curObj, 0.0f, 65535.0f, (void**)&port, true);

	interface_staticLabel("Addresse:", relativePlacement(0.0f, 0.0f, 1.0f, 0.2f), ANCHOR_CENTER);

	interface_staticLabel("Port:", relativePlacement(0.0f, 0.4f, 1.0f, 0.2f), ANCHOR_CENTER);

	interface_staticButton("Connexion", relativePlacement(0.0f, 0.8f, 1.0f, 0.2f), connect);

	interface_popBlock();

	interface_updateLayout();
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

			if (!net.inputOnly)
			{
				if (player == myPlayer)
				{ // Only take the server's data if it differs greatly from ours
					if (vectorDistance(myPlayer->pos, net.content.player.pos) > 2.0f)
					{
						*player = net.content.player;
					}
				}
				else
				{
					*player = net.content.player;
				}
			}
			else if (player != myPlayer)
			{
				player->input = net.content.input;
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
		showLoginInterface();
		time_sync(time.currentTime);
	}

    BG_gameLoop(time.deltaTimeSeconds);

    updateCamera(input);

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

			sprintf(healthString, "Health: %d", myPlayer->health);

			if (myPlayer->health == 0)
			{ // That's... extreme?
				CL_disconnectFromServer(NULL);
				engine_shutdown();
			}
        }
    }
}

void drawStars()
{
	uint i;

	glPushMatrix();
	if (myPlayer)
	{ // Create some kind of parallax so the player never goes past the stars (unless he tries very hard)
		// This makes the stars disappear if you get too close for some reason
		glTranslatef(0.9f * myPlayer->pos[0], 0.9f * myPlayer->pos[1], 0.9f * myPlayer->pos[2]);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for (i = 0; i < NB_STARS; i++)
	{
		glVertex3fv(_stars[i]);
	}
	glEnd();

	glPopMatrix();
}

void renderFunc(void)
{
	uint i;
	float viewMatrix[16];

	engine_getViewMatrix(viewMatrix);

	if (CL_connected())
	{
		renderMesh(_skybox, viewMatrix);

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
