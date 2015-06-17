#include "utils_string.h"
#include "utils.h"

#include "utils_bytestream.h"
#include <string.h>

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions

char *newString(char *s)
{
	int length = strlen(s);
	char *r = (char*)mem_alloc(sizeof(char)*(length+1));
	strncpy(r, s, length);
	r[length] = '\0';
	return r;
}

char *newString2(char *s, unsigned int len)
{
	unsigned int sLen = strlen(s);
	int length = (sLen<len)?sLen:len;
	char *r = (char*)mem_alloc(sizeof(char)*(len+1));
	strncpy(r, s, length);
	r[length] = '\0';
	return r;
}

char *strFromInt(int n)
{
	uint len = nbDigits(n);
	char *res = (char*)mem_alloc(sizeof(char)*len+1);
	sprintf(res, "%d", n);
	return res;
}

char *strFromFloat(float n)
{
	uint len = nbDigits((int)n) + 10;	// Something like that...
	char *res = (char*)mem_alloc(sizeof(char)*len+1);
	sprintf(res, "%f", n);
	return res;
}

char *strFromVec(float vec[3])
{
	char *res = (char*)mem_alloc(sizeof(char)*48); // Sounds about right
	sprintf(res, "%f %f %f", vec[0], vec[1], vec[2]);
	return res;
}

char *strFromBinary(byte *bin)
{
	uint len = mem_size(bin);
	char *res;
	uint i;

	if (!len) len = 1;

	res = (char*)mem_alloc(sizeof(char)*(len*9)); // len * (8 digits + one space or \0)

	for (i = 0; i < len; i++)
	{
		uint j;
		for (j = 0; j < 8; j++)
		{
			if (bin[i] & (1 << (7-j)))
			{
				res[9*i+j] = '1';
			}
			else
			{
				res[9*i+j] = '0';
			}
		}
		res[9*i+j] = ' ';
	}
	res[9*len-1] = '\0';
	return res;
}

void strip(char *s, char c)
{
	unsigned int i=0,j=0;
	unsigned int len = strlen(s);
	short inString = 0;
	
	while (j < len)
	{
		if ((s[j] == '\"' || s[j] == '\'') && (j == 0 || s[j-1] != '\\') && s[j] != c)
			inString = !inString;

		if (!inString)
		{
			if (s[j] == c)
			{
				j++;
				continue;
			}
		}
		s[i] = s[j];
		i++;
		j++;
	}
	s[i] = '\0';
}