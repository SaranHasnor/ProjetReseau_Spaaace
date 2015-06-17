#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>

void keyDownFunc(uchar key)
{
	if (key == 27)
	{
		shutdownEngine();
	}
}

void keyUpFunc(uchar key)
{

}

void mouseDownFunc(ushort button)
{

}

void mouseUpFunc(ushort button)
{

}

void updateFunc(timeStruct_t time, inputStruct_t input)
{

}

void renderFunc(void)
{
	drawAxis();
}

int main(int argc, char **argv)
{
	engineListener_t listener;

	listener.keyDownFunc = keyDownFunc;
	listener.keyUpFunc = keyUpFunc;
	listener.mouseDownFunc = mouseDownFunc;
	listener.mouseUpFunc = mouseUpFunc;
	listener.renderFunc = renderFunc;
	listener.updateFunc = updateFunc;

	runEngine(argc, argv, listener);

	return 0;
}
