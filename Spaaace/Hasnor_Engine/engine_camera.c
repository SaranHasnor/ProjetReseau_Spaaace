#include <GL/glut.h>
#include "engine_camera.h"
#include <utils.h>
#include <utils_vector.h>
#include <utils_matrix.h>

float _camPos[3];
float _camAngle[3];

float _moveDir[3];

bool enableCameraMovement = false;
float cameraSpeed = 1.0f;

float _renderProjectionMatrix[16];
float _interfaceProjectionMatrix[16];
float _renderModelViewMatrix[16];

// TEMP
int _screenWidth, _screenHeight;

void initCamera()
{
	vectorSet(_moveDir, 0.0f, 0.0f, 0.0f);
	vectorSet(_camPos, DEFAULT_CAMERA_POSITION);
	vectorSet(_camAngle, DEFAULT_CAMERA_ANGLE);
}

void setCameraSize(int width, int height)
{
	mat_perspective(_renderProjectionMatrix, 60.0f, (float)width/(float)height, 1.0f, 200.0f);
	mat_orthographic(_interfaceProjectionMatrix, width, height, -200.0f, 200.0f); // FIXME
	_screenWidth = width;
	_screenHeight = height;
}

void setCameraPosition(float pos[3])
{
	vectorCopy(_camPos, pos);
}

void setCameraMovementDirection(float dir[3])
{ // We expect dir to be normalized, but I guess its length can be used as a speed multiplier
	vectorCopy(_moveDir, dir);
}

void cameraInput(inputStruct_t input)
{
	if (input.mouseButtons & INPUT_MOUSELEFT)
	{
		_camAngle[0] += input.mouseDelta[1];
		_camAngle[1] -= input.mouseDelta[0];

		if (_camAngle[0] < -85)
			_camAngle[0] = -85;
		if (_camAngle[0] > 85)
			_camAngle[0] = 85;

		if (_camAngle[1] < -180)
			_camAngle[1] += 360;
		if (_camAngle[1] > 180)
			_camAngle[1] -= 360;
	}

	if (input.key_timeHeld('z') > 0)
	{
		_moveDir[1] += 1.0f;
	}
	else if (input.key_timeHeld('Z') > 0)
	{
		_moveDir[1] += 2.0f;
	}

	if (input.key_timeHeld('s') > 0)
	{
		_moveDir[1] -= 1.0f;
	}
	else if (input.key_timeHeld('S') > 0)
	{
		_moveDir[1] -= 2.0f;
	}

	if (input.key_timeHeld('d') > 0)
	{
		_moveDir[0] += 1.0f;
	}
	else if (input.key_timeHeld('D') > 0)
	{
		_moveDir[0] += 2.0f;
	}

	if (input.key_timeHeld('q') > 0)
	{
		_moveDir[0] -= 1.0f;
	}
	else if (input.key_timeHeld('Q') > 0)
	{
		_moveDir[0] -= 2.0f;
	}

	if (input.key_timeHeld(' ') > 0)
	{
		_moveDir[2] += 1.0f;
	}

	if (input.key_timeHeld('x') > 0 || input.key_timeHeld('X') > 0)
	{
		_moveDir[2] -= 1.0f;
	}
}

void updateCamera(timeStruct_t time, inputStruct_t input)
{
	float forward[3], right[3];
	float movement[3];
	
	if (enableCameraMovement)
	{
		cameraInput(input);
	}

	AngleVectors(_camAngle, forward, right, NULL);

	// Update the camera position (cheap method because math is hard)
	vectorScale(movement, _moveDir[2], axis[2]);
	vectorMA(movement, movement, _moveDir[1], forward);
	vectorMA(movement, movement, _moveDir[0], right);
	
	vectorMA(_camPos, _camPos, cameraSpeed * time.deltaTimeSeconds, movement);
	vectorSet(_moveDir, 0.0f, 0.0f, 0.0f);

	mat_viewModel(_renderModelViewMatrix, _camPos, _camAngle);
}

void positionGLCameraForRender()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_renderProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_renderModelViewMatrix);
}

void positionGLCameraForInterface()
{
	glMatrixMode(GL_PROJECTION);
	//glLoadMatrixf(_interfaceProjectionMatrix);
	glLoadIdentity();
	gluOrtho2D(0.0, _screenWidth, _screenHeight, 0.0); // TEMP

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}