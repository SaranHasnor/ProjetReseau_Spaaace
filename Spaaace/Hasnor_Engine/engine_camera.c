#include <GL/glut.h>
#include "engine_camera.h"
#include <utils.h>
#include <utils_vector.h>

float _camPos[3];
float _camAngle[3];

float _moveDir[3];

bool enableCameraMovement = false;
float cameraSpeed = 1.0f;

void initCamera()
{
	vectorSet(_moveDir, 0.0f, 0.0f, 0.0f);
	vectorSet(_camPos, DEFAULT_CAMERA_POSITION);
	vectorSet(_camAngle, DEFAULT_CAMERA_ANGLE);
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
	
#if USE_NEW_CAMERA_MOVEMENT
	vectorScale(movement, time.deltaTimeSeconds, movement);
	vectorAdd(_camPos, _camPos, movement);
	vectorScale(_moveDir, 0.95f, _moveDir); // Science
#else
	vectorMA(_camPos, _camPos, cameraSpeed * time.deltaTimeSeconds, movement);
	vectorSet(_moveDir, 0.0f, 0.0f, 0.0f);
#endif
}

void positionGLCameraForRender()
{
	float forward[3], right[3], camLookPos[3];

	// Calculate the camera's facing direction
	AngleVectors(_camAngle, forward, right, NULL);
	vectorScale(forward, 100, forward);
	vectorAdd(camLookPos, forward, _camPos);

	// Set the camera (I want to put this out of the loop and move/rotate the world instead)
	gluLookAt((double)_camPos[0], (double)_camPos[1], (double)_camPos[2],
		(double)camLookPos[0], (double)camLookPos[1], (double)camLookPos[2],
		(double)axis[2][0], (double)axis[2][1], (double)axis[2][2]);
}