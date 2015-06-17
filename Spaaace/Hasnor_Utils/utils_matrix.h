#ifndef HASNOR_MATRIX_IMPORTED
#define HASNOR_MATRIX_IMPORTED

void mat_perspective(float mat[16], float fov, float width, float height, float near, float far);
void mat_identity(float mat[16]);
void mat_multiply(float out[16], float a[16], float b[16]);

#endif