#ifndef HASNOR_ENGINE_CAMERA_IMPORTED
#define HASNOR_ENGINE_CAMERA_IMPORTED

#include "engine_callbacks.h"
#include <utils_types.h>

/*
engine_camera

- Handles the camera's position
*/

#define DEFAULT_CAMERA_POSITION 0.0f,-10.0f,0.0f
#define DEFAULT_CAMERA_ANGLE 0.0f,90.0f,0.0f

extern bool enableCameraMovement;
extern float cameraSpeed;

void initCamera();
void setCameraSize(int width, int height);
void setCameraPosition(float pos[3]);
void setCameraMovementDirection(float dir[3]);
void updateCamera(timeStruct_t time, inputStruct_t input);
void positionGLCameraForRender();
void positionGLCameraForInterface();

#endif