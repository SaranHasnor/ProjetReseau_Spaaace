#include <stdio.h>
#include <engine.h>
#include <engine_interface.h>
#include <engine_utils.h>
#include <game.h>

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

void initEngine()
{
	interface_staticLabel(":D", dynamicPlacement(0, 0, 0, 0), ANCHOR_CENTER);
}

void updateFunc(timeStruct_t time, inputStruct_t input)
{
	if (time.deltaTime == time.currentTime)
	{
		initEngine();
	}
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

	runEngine(argc, argv, 1200, 600, "Spaaace", listener);

	return 0;
}
