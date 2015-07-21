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

float playerPosition[3] = { 0, 0, 0 };
void updateCamera(inputStruct_t input)
{
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
        playerPosition[1] += 0.01f;
	}

	if (input.key_timeHeld('s') > 0)
	{
        playerPosition[1] -= 0.01f;
	}

	if (input.key_timeHeld('d') > 0)
	{
        playerPosition[0] += 0.01f;
	}

	if (input.key_timeHeld('q') > 0)
	{
        playerPosition[0] -= 0.01f;
	}

	if (input.key_timeHeld(' ') > 0)
	{
        playerPosition[2] += 0.01f;
	}

	if (input.key_timeHeld('x') > 0)
	{
        playerPosition[2] -= 0.01f;
	}

    //engine_setCameraPosition(playerPosition);
	engine_setCameraVelocity(velocity);


    //testMesh->origin[0] = playerPosition[0];
    //testMesh->origin[1] = playerPosition[1];
    //testMesh->origin[2] = playerPosition[2];
}

void initEngine()
{
    setupNetwork();
    InitializePlayerClient();
    CreateNewPlayer(false);
}

void updateFunc(timeStruct_t time, inputStruct_t input)
{
	if (time.deltaTime == time.currentTime)
	{
		initEngine();
	}

	//testMesh->origin[2] += 0.1f * time.deltaTimeSeconds;
	//mat_rotation(testMesh->rotation, 20.0f * testMesh->origin[2], 50.0f * testMesh->origin[2], 0.0f);

	updateCamera(input);
}

void renderFunc(void)
{
	float viewMatrix[16];

	drawAxis();

	engine_getViewMatrix(viewMatrix);

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
