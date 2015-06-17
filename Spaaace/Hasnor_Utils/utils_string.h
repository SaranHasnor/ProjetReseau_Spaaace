#ifndef HASNOR_STRING_DEFINED
#define HASNOR_STRING_DEFINED

#include "utils_types.h"
#include "utils_bytestream.h"

/*
utils_string

- Functions and structures for creating and parsing arrays of characters
*/

typedef struct {
	char			*s;
	unsigned int	len;
} string;

char *newString(char *s);
char *newString2(char *s, unsigned int length);
char *strFromInt(int n);
char *strFromFloat(float n);
char *strFromVec(float vec[3]);
char *strFromBinary(byte *bin);

void strip(char *s, char c);

#endif