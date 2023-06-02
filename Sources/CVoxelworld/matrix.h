#ifndef _matrix_h_
#define _matrix_h_

#include <cglm/cglm.h>

void mat_apply(float *data, mat4 matrix, int count, int offset, int stride);

void frustum_planes(vec4 planes[6], int radius, mat4 matrix);

void set_matrix_2d(mat4 matrix, int width, int height);

void set_matrix_3d(mat4 matrix, int width, int height, float x, float y, float z, float rx, float ry, float fov, int ortho, int radius);

void set_matrix_item(mat4 matrix, int width, int height, int scale);

#endif
