#include "engine_window.h"
#include "engine_input.h"
#include "engine_interface.h"
#include "engine_camera.h"
#include <GL\glut.h>
#include <utils.h>

window_t window;
engineListener_t _listener;

void update(int prevTime)
{
	const int curTime = glutGet(GLUT_ELAPSED_TIME);
	timeStruct_t time;
	inputStruct_t input;

	time.currentTime = curTime;
	time.deltaTime = curTime-prevTime;
	time.deltaTimeSeconds = time.deltaTime / 1000.0f;

	//printf("FPS: %f\n", 1.0f / time.deltaTimeSeconds);

	updateInput(&input);
	
	glutSetWindow(window.id);
	updateCamera(time, input);
	_listener.updateFunc(time, input);

	glutTimerFunc(10, update, curTime);
	glutPostRedisplay();
}

void display(void)
{
	// Frame initialization
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Camera positioning
	positionGLCameraForRender();

	// Rendering
	_listener.renderFunc();

	// Frame saving and rendering
	glutSwapBuffers();
	glFlush();
}

void runEngine(int argc, char **argv, engineListener_t listener)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	window.width = DEFAULT_WIDTH;
	window.height = DEFAULT_WIDTH;
	window.name = newString("Test");

	createWindow(&window);

	glutDisplayFunc(display);
	
	initInput(listener);

	_listener = listener;

	initCamera();
	enableCameraMovement = true;

	update(0);

	glutMainLoop();
}

void shutdownEngine()
{
	// Force the engine to shutdown
	quit(); // temp
}