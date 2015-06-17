#include "time.h"
#include <time.h>	// Not confusing at all

clock_t start;

void time_init()
{
	start = clock();
}

double time_current_sec()
{
	clock_t current = clock();

	return ((double)(current - start) / CLOCKS_PER_SEC);
}

long time_current_ms()
{
	return (long)(time_current_sec() * 1000);
}