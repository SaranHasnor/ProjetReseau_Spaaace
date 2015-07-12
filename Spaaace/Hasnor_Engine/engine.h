#ifndef HASNOR_ENGINE_IMPORTED
#define HASNOR_ENGINE_IMPORTED

#include "engine_callbacks.h"

void runEngine(int argc, char **argv, int windowWidth, int windowHeight, char *windowName, engineListener_t listener);
void shutdownEngine();

#endif