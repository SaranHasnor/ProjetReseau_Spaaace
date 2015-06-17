#include "utils_matrix.h"
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

void mat_perspective(float mat[16], float fov, float width, float height, float near, float far)
{
	float aspect = width / height;
	float xymax = near * tanf(fov * (3.141592f / 360.0f));
	float ymin = -xymax;
	float xmin = -xymax;
	float depth = far - near;
	float q = -(far + near) / depth;
	float qn = -2.0f * near / depth;
	float w, h;

	width = xymax - xmin;
	height = xymax - ymin;

	w = (2.0f * near / width) / depth;
	h = 2.0f * near / height;

	memset(mat, 0, sizeof(float) * 16);
	mat[0] = w;
	mat[5] = h;
	mat[10] = q;
	mat[11] = -1.0f;
	mat[14] = qn;
}

void mat_identity(float mat[16])
{
	int i;
	for (i = 0; i < 16; i++)
	{
		if ((i % 5) == 0)
		{
			mat[i] = 1.0f;
		}
		else
		{
			mat[i] = 0.0f;
		}
	}
}

void mat_multiply(float out[16], float a[16], float b[16])
{
	int i,j;
	for (j=0; j<16; j+=4)
	{
		for (i=0; i<4; i++)
		{
			float val = 0.0f;
			int k;
			for (k=0; k<4;k++)
			{
				val += a[i+(4*k)] * b[k+j];
			}
			out[i+j] = val;
		}
	}
}