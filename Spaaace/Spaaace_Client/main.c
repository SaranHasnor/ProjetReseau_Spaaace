#include <stdio.h>
#include <engine.h>
#include <engine_interface.h>
//#include <engine_render.h>
#include <engine_utils.h>
#include <utils_matrix.h>
#include <game.h>
#include "PlayerClient.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

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
        velocity[1] += 1.00f;
	}

	if (input.key_timeHeld('s') > 0)
	{
        velocity[1] -= 1.00f;
	}

	if (input.key_timeHeld('d') > 0)
	{
        velocity[0] += 1.00f;
	}

	if (input.key_timeHeld('q') > 0)
	{
        velocity[0] -= 1.00f;
	}

	if (input.key_timeHeld(' ') > 0)
	{
        velocity[2] += 1.00f;
	}

	if (input.key_timeHeld('x') > 0)
	{
        velocity[2] -= 1.00f;
	}

    //engine_setCameraPosition(playerPosition);
	engine_setCameraVelocity(velocity);
    engine_getCameraPosition(&cameraPosition);
    PlayerWantToMove(cameraPosition);

    //testMesh->origin[0] = playerPosition[0];
    //testMesh->origin[1] = playerPosition[1];
    //testMesh->origin[2] = playerPosition[2];
}

void initEngine()
{
    setupNetwork();
    PlayerClient_init();
}

void MessageListener(networkUpdate_t update)
{

    string strMessageByte;
    string strMessage;
    string headerMessage;
    string messageContent;

    string_initStr(&strMessage, "");
    string_initStr(&headerMessage, "");
    string_initStr(&messageContent, "");

    for (int i = 0; i < update.count; i++)
    {
        printMessage(update.messages[i]);
        char buffer[128]; // taille max du message
        bytestream_read(&update.messages[i].content, buffer, update.messages[i].content.len);
        string_initStr(&strMessageByte, buffer);
        str_substring(strMessageByte, '!', &strMessage);
        str_substring(strMessage, ':', &headerMessage);
        if (strcmp(headerMessage.s , "New Player") == 0)
        {
            str_substringIndex(strMessage, headerMessage.len + 1, strMessage.len, &messageContent);
            CreateNewPlayerStringMessage(messageContent);
        }
        else if (strcmp(headerMessage.s, "PlayerPosition") == 0)
        {
            str_substringIndex(strMessage, headerMessage.len + 1, strMessage.len, &messageContent);
            MovePlayerMessage(messageContent);
        }
    }
}

void updateFunc(timeStruct_t time, inputStruct_t input)
{
    networkUpdate_t update;

	if (time.deltaTime == time.currentTime)
	{
		initEngine();
	}

	//testMesh->origin[2] += 0.1f * time.deltaTimeSeconds;
	//mat_rotation(testMesh->rotation, 20.0f * testMesh->origin[2], 50.0f * testMesh->origin[2], 0.0f);

	updateCamera(input);
    if (CL_connected())
    {
        CL_update(&update);
        if (update.count > 0)
        {
            MessageListener(update);
        }
    }
}

void renderFunc(void)
{
	float viewMatrix[16];

	drawAxis();

	engine_getViewMatrix(viewMatrix);

    RenderClient(viewMatrix);
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
