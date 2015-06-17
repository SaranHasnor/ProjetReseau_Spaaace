#include <GL/glut.h>
#include "engine_input.h"
#include <utils.h>

typedef struct inputKey_s {
	unsigned char		key;
	int					pressTime;

	struct inputKey_s	*next;
} inputKey_t;

uint _mouseButtons;
int _mousePos[2];
int _lastUpdatePos[2];
inputKey_t *_heldKeys = NULL;

engineListener_t _inputListener;

inputKey_t *heldKey(unsigned char key)
{
	inputKey_t *curKey = _heldKeys;
	while (curKey)
	{
		if (curKey->key == key)
		{
			return curKey;
		}
		curKey = curKey->next;
	}
	return NULL;
}

void appendHeldKey(unsigned char key, int currentTime)
{
	inputKey_t **curKey = &_heldKeys;
	
	while (*curKey)
	{
		if ((*curKey)->key == key)
		{ // Already have it
			return;
		}
		/*else if (charsEqualCaseInsensitive((*curKey)->key, key))
		{ // Already have it but the case is different
			(*curKey)->key = key;
			return;
		}*/
		curKey = &(*curKey)->next;
	}
	
	*curKey = (inputKey_t*)mem_alloc(sizeof(inputKey_t));
	(*curKey)->key = key;
	(*curKey)->pressTime = currentTime;
	(*curKey)->next = NULL;
}

void deleteHeldKey(unsigned char key)
{
	inputKey_t **curKey = &_heldKeys;
	while (*curKey)
	{
		if ((*curKey)->key == key)
		{
			inputKey_t *temp = *curKey;
			*curKey = temp->next;
			mem_free(temp);
			return;
		}
		curKey = &(*curKey)->next;
	}
}

int keyTimeHeld(unsigned char key)
{
	inputKey_t *curKey = _heldKeys;
	while (curKey)
	{
		if (curKey->key == key)
		{
			return curKey->pressTime;
		}
		curKey = curKey->next;
	}
	return 0;
}

void motion(int x, int y)
{
	_mousePos[0] = x;
	_mousePos[1] = y;
}

void mouse(int button, int state, int x, int y)
{
	ushort actualButton;
	if (button == GLUT_LEFT_BUTTON)
	{
		actualButton = INPUT_MOUSELEFT_BUTTON;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		actualButton = INPUT_MOUSERIGHT_BUTTON;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		actualButton = INPUT_MOUSEMIDDLE_BUTTON;
	}
	else
	{ // ???
		assert(0);
		actualButton = -1;
	}

	if (state == GLUT_DOWN)
	{
		_mouseButtons |= (1 << actualButton);
	}
	else
	{
		_mouseButtons &= ~(1 << actualButton);
	}

	// Might as well
	_mousePos[0] = x;
	_mousePos[1] = y;

	if (state == GLUT_DOWN)
	{
		_inputListener.mouseDownFunc(actualButton);
	}
	else
	{
		_inputListener.mouseUpFunc(actualButton);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	appendHeldKey(key, glutGet(GLUT_ELAPSED_TIME));

	_inputListener.keyDownFunc(key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	deleteHeldKey(key);

	_inputListener.keyUpFunc(key);
}

void initInput(engineListener_t listener)
{
	_inputListener = listener;

	_lastUpdatePos[0] = _lastUpdatePos[1] = 0;

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);
}

void updateInput(inputStruct_t *input)
{
	input->mouseButtons = _mouseButtons;

	input->mousePos[0] = _mousePos[0];
	input->mousePos[1] = _mousePos[1];

	input->mouseDelta[0] = _mousePos[0] - _lastUpdatePos[0];
	input->mouseDelta[1] = _mousePos[1] - _lastUpdatePos[1];

	_lastUpdatePos[0] = _mousePos[0];
	_lastUpdatePos[1] = _mousePos[1];

	input->key_timeHeld = keyTimeHeld; // Not necessary to do that every frame
}