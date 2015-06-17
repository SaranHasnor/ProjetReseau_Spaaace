#include "engine_window.h"
#include <GL/glut.h>
#include <utils.h>

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glPointSize(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);
}

void reshape(int w, int h)
{
	short fixW=0, fixH=0;

	if (w != DEFAULT_WIDTH || h != DEFAULT_HEIGHT)
	{ // For now, no resizing because it breaks everything
		glutReshapeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		return;
	}

	if (h != DEFAULT_HEIGHT)
	{ // Can't change that, or it breaks the interface
		fixH = 1;
	}

	if (w < INTERFACE_WIDTH)
	{ // Too small
		fixW = 1;
	}

	if (fixH || fixW)
	{
		glutReshapeWindow((fixW?INTERFACE_WIDTH:w), (fixH?DEFAULT_HEIGHT:h));
		return;
	}

	glViewport(0, 0,(GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
	//gluOrtho2D(0.0, w, h, 0.0);
}

void createWindow(window_t *window)
{
	const int screenX = glutGet(GLUT_SCREEN_WIDTH);
	const int screenY = glutGet(GLUT_SCREEN_HEIGHT);

	// Initializing GLUT
	glutInitWindowSize(window->width, window->height);
	glutInitWindowPosition((screenX-window->width)/2, (screenY-window->height)/2);
	window->id = glutCreateWindow(window->name);

	initRendering();

	glutReshapeFunc(reshape);
}