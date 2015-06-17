#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

/*
utils_misc

- Utility functions that don't belong anywhere else
*/

int isInRange(float min, float val, float max);
unsigned int nbDigits(int n);

int randomIntBetween(int min, int max);
float randomValueBetween(float min, float max);

float atLeastOne(float x);
void strcpy_safe(char *dst, char *src);

bool charsEqualCaseInsensitive(char a, char b);

void quit();

#endif