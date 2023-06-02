#include <math.h>
#include "config.h"
#include "matrix.h"
#include "utils.h"

void mat_apply(float *data, mat4 matrix, int count, int offset, int stride) {
    vec4 vec = GLM_VEC4_ZERO;
    for (int i = 0; i < count; i++) {
		float *p = data + offset + stride * i;
        float *d = p;
        vec[0] = *(d++); vec[1] = *(d++); vec[2] = *(d++);
		glm_mat4_mulv(matrix, vec, vec);
        d = p;
        *(d++) = vec[0]; *(d++) = vec[1]; *(d++) = vec[2];
    }
}

void frustum_planes(vec4 planes[6], int radius, mat4 matrix) {
    float znear = 0.125;
    float zfar = radius * 32 + 64;
	glm_frustum_planes(matrix, planes);
	glm_vec4_scale(planes[4], 0.125, planes[4]);
	glm_vec4_scale(planes[5], radius * 32 + 64, planes[5]);
}

void set_matrix_2d(mat4 matrix, int width, int height) {
	glm_ortho(0, width, 0, height, -1, 1, matrix);
}

void set_matrix_3d(
    mat4 matrix, int width, int height,
    float x, float y, float z, float rx, float ry,
    float fov, int ortho, int radius)
{
    mat4 a = GLM_MAT4_IDENTITY;
    mat4 b = GLM_MAT4_IDENTITY;
    float aspect = (float)width / height;
    float znear = 0.125;
    float zfar = radius * 32 + 64;
	glm_translate_make(a, (vec3){ -x, -y, -z });
	glm_mul(b, a, a);
	glm_rotate_make(b, -ry, (vec3){ cosf(rx), 0, sinf(rx) });
	glm_mul(b, a, a);
	glm_rotate_make(b, rx, (vec3){ 0, 1, 0 });
	glm_mul(b, a, a);
    if (ortho)
		glm_ortho(-ortho * aspect, ortho * aspect, -ortho, ortho, -zfar, zfar, b);
    else
		glm_perspective(RADIANS(fov), aspect, znear, zfar, b);
	glm_mul(b, a, a);
	glm_mat4_copy(a, matrix);
}

void set_matrix_item(mat4 matrix, int width, int height, int scale) {
    mat4 a = GLM_MAT4_IDENTITY;
    mat4 b;
    float aspect = (float)width / height;
    float size = 64 * scale;
    float box = height / size / 2;
    float xoffset = 1 - size / width * 2;
    float yoffset = 1 - size / height * 2;
	glm_rotate_make(b, PI / 4, (vec3){ 0, 1, 0 });
	glm_mul(b, a, a);
	glm_rotate_make(b, PI / 10, (vec3){ 1, 0, 0 });
	glm_mul(b, a, a);
	glm_ortho(-box * aspect, box * aspect, -box, box, -1, 1, b);
	glm_mul(b, a, a);
	glm_translate_make(b, (vec3){ -xoffset, -yoffset, 0 });
	glm_mul(b, a, a);
	glm_mat4_copy(a, matrix);
}
