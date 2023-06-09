#include <math.h>
#include "cube.h"
#include "item.h"
#include "matrix.h"
#include "utils.h"
#include "mesh.h"

void make_cube_faces(size_t index, unsigned short *indices, struct vertex_s *vertices, float ao[6][4], float light[6][4], int left, int right, int top, int bottom, int front, int back, int wleft, int wright, int wtop, int wbottom, int wfront, int wback, float px, float py, float pz, float n) {

	int faces[6] = { left, right, top, bottom, front, back };
	int tiles[6] = { wleft, wright, wtop, wbottom, wfront, wback };
	int i = -1;
	int j = 0;

	// Prepare the templates

#define flipped \
	ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2]

#define triangle(t0, t1, t2) \
	*indices++ = index + t0; \
	*indices++ = index + t1; \
	*indices++ = index + t2; \
	index += 3;

	vec4 normalv = GLM_VEC4_ZERO;
#define normals(x, y, z) \
	normalv[0] = x; \
	normalv[1] = y; \
	normalv[2] = z

	float du, dv;
#define uvs \
	du = tiles[i] % 16 * s; \
	dv = tiles[i] / 16 * s

	float s = 0.0625;
	float a = 1 / 2048.0;
	float b = s - a;
#define vertex(x, y, z, u, v) \
	*vertices++ = (struct vertex_s) { \
		.position = { n * x, n * y, n * z }, \
		.normal = { normalv[0], normalv[1], normalv[2] }, \
		.uv = { du + (u ? b : a), dv + (v ? b : a) }, \
		.ao = ao[i][j], \
		.light = light[i][j], \
	}; j++;

#define face \
	j = 0; \
	if (faces[++i] != 0)

	//TODO: Make this use templates

	face {
		uvs;
		normals(-1, 0, 0);

		if (flipped) {
			triangle(0, 1, 2);
			triangle(1, 3, 2);
		} else {
			triangle(0, 3, 2);
			triangle(0, 1, 3);
		}

		vertex(-1, -1, -1, 0, 0);
		vertex(-1, -1, +1, 1, 0);
		vertex(-1, +1, -1, 0, 1);
		vertex(-1, +1, +1, 1, 1);
	}

	face {
		uvs;
		normals(+1, 0, 0);

		if (flipped) {
			triangle(0, 2, 1);
			triangle(2, 3, 1);
		} else {
			triangle(0, 3, 1);
			triangle(0, 2, 3);
		}

		vertex(+1, -1, -1, 1, 0);
		vertex(+1, -1, +1, 0, 0);
		vertex(+1, +1, -1, 1, 1);
		vertex(+1, +1, +1, 0, 1);
	}

	face {
		uvs;
		normals(0, +1, 0);

		if (flipped) {
			triangle(0, 1, 2);
			triangle(1, 3, 2);
		} else {
			triangle(0, 3, 2);
			triangle(0, 1, 3);
		}

		vertex(-1, +1, -1, 0, 1);
		vertex(-1, +1, +1, 0, 0);
		vertex(+1, +1, -1, 1, 1);
		vertex(+1, +1, +1, 1, 0);
	}

	face {
		uvs;
		normals(0, -1, 0);

		if (flipped) {
			triangle(0, 2, 1);
			triangle(2, 3, 1);
		} else {
			triangle(0, 3, 1);
			triangle(0, 2, 3);
		}

		vertex(-1, -1, -1, 0, 0);
		vertex(-1, -1, +1, 0, 1);
		vertex(+1, -1, -1, 1, 0);
		vertex(+1, -1, +1, 1, 1);
	}

	face {
		uvs;
		normals(0, 0, -1);

		if (flipped) {
			triangle(0, 1, 2);
			triangle(1, 3, 2);
		} else {
			triangle(0, 3, 2);
			triangle(0, 1, 3);
		}

		vertex(-1, -1, -1, 0, 0);
		vertex(-1, +1, -1, 0, 1);
		vertex(+1, -1, -1, 1, 0);
		vertex(+1, +1, -1, 1, 1);
	}

	face {
		uvs;
		normals(0, 0, +1);

		if (flipped) {
			triangle(0, 2, 1);
			triangle(2, 3, 1);
		} else {
			triangle(0, 3, 1);
			triangle(0, 2, 3);
		}

		vertex(-1, -1, +1, 1, 0);
		vertex(-1, +1, +1, 1, 1);
		vertex(+1, -1, +1, 0, 0);
		vertex(+1, +1, +1, 0, 1);
	}

#undef face
#undef uvs
#undef normals
#undef triangle
#undef flipped
#undef vertex
}

void make_cube(size_t index, unsigned short *indices, struct vertex_s *vertices, float ao[6][4], float light[6][4], int left, int right, int top, int bottom, int front, int back, float x, float y, float z, float n, int w) {
    int wleft = blocks[w][0];
    int wright = blocks[w][1];
    int wtop = blocks[w][2];
    int wbottom = blocks[w][3];
    int wfront = blocks[w][4];
    int wback = blocks[w][5];
    make_cube_faces(
        index, indices, vertices, ao, light,
        left, right, top, bottom, front, back,
        wleft, wright, wtop, wbottom, wfront, wback,
        x, y, z, n);
}

void make_plant(size_t index, unsigned short *indices, struct vertex_s *vertices, float ao, float light, float px, float py, float pz, float n, int w, float rotation) {
	// Calculate UVs from bottom-left to top-right
	float s = 0.0625;
	float du = plants[w] % 16 * s;
	float dv = plants[w] / 16 * s;

	// Prepare normals rotation
	mat4 rotate = GLM_MAT4_IDENTITY;
	glm_rotate_y(rotate, RADIANS(rotation), rotate);

	// Prepare position translation
	mat4 translate = GLM_MAT4_IDENTITY;
	glm_translate_make(translate, (vec3){ px, py, pz });
	glm_mul(translate, rotate, translate);

	// Prepare templates

#define triangle(t0, t1, t2) \
	*indices++ = index + t0; \
	*indices++ = index + t1; \
	*indices++ = index + t2; \
	index += 3

	vec4 normalv = GLM_VEC4_ZERO;
#define normals(x, y, z) \
	glm_mat4_mulv(rotate, (vec4){ x, y, z, 0 }, normalv)

	vec4 positionv = GLM_VEC4_ZERO;
#define vertex(x, y, z, u, v) \
	glm_mat4_mulv(translate, (vec4){ n * x, n * y, n * z, 0 }, positionv); \
	*vertices++ = (struct vertex_s) { \
		.position = { positionv[0], positionv[1], positionv[2] }, \
		.normal = { normalv[0], normalv[1], normalv[2] }, \
		.uv = { du + u * s, dv + v * s }, \
		.ao = ao, \
		.light = light, \
	}

	// Face #0
	normals(-1, 0, 0);
	vertex(0, -1, -1, 0, 0);
	vertex(0, -1, +1, 1, 0);
	vertex(0, +1, -1, 0, 1);
	vertex(0, +1, +1, 1, 1);
	triangle(0, 3, 2);
	triangle(0, 1, 3);

	// Face #1 (mirror of #0)
	normals(+1, 0, 0);
	vertex(0, -1, -1, 1, 0);
	vertex(0, -1, +1, 0, 0);
	vertex(0, +1, -1, 1, 1);
	vertex(0, +1, +1, 0, 1);
	triangle(0, 3, 1);
	triangle(0, 2, 3);

	// Face #2
	normals(0, 0, -1);
	vertex(-1, -1, 0, 0, 0);
	vertex(-1, +1, 0, 0, 1);
	vertex(+1, -1, 0, 1, 0);
	vertex(+1, +1, 0, 1, 1);
	triangle(0, 3, 2);
	triangle(0, 1, 3);

	// Face #3 (mirror of #2)
	normals(0, 0, +1);
	vertex(-1, -1, 0, 1, 0);
	vertex(-1, +1, 0, 1, 1);
	vertex(+1, -1, 0, 0, 0);
	vertex(+1, +1, 0, 0, 1);
	triangle(0, 3, 1);
	triangle(0, 2, 3);

#undef normals
#undef triangle
#undef vertex
}

void generate_cross_geometry(geometry_t geometry, int id, float ao, float light, float x, float y, float z, float rotation) {
	// Calculate the UV offset
	float du = plants[id] % 16 / 16;
	float dv = plants[id] / 16 / 16;
	geometry_uvs_offset(geometry, du, dv);

	// Prepare the lighting information
	geometry_lighting(geometry, ao, light);

	// Prepare normals rotation
	mat4 rotate = GLM_MAT4_IDENTITY;
	glm_rotate_y(rotate, RADIANS(rotation), rotate);

	// Face #1

	geometry_normals(geometry, -1, 0, 0);
	geometry_triangle(geometry, 0, 3, 2);
	geometry_triangle(geometry, 0, 1, 3);

	geometry_position(geometry, 0, -1, -1);
	geometry_uvs(geometry, 0, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, -1, +1);
	geometry_uvs(geometry, 1, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, -1);
	geometry_uvs(geometry, 0, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, +1);
	geometry_uvs(geometry, 1, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	// Face #2 (mirror of #1)

	geometry_normals(geometry, +1, 0, 0);
	geometry_triangle(geometry, 0, 3, 1);
	geometry_triangle(geometry, 0, 2, 3);

	geometry_position(geometry, 0, -1, -1);
	geometry_uvs(geometry, 1, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, -1, +1);
	geometry_uvs(geometry, 0, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, -1);
	geometry_uvs(geometry, 1, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, +1);
	geometry_uvs(geometry, 0, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	// Face #3

	geometry_normals(geometry, 0, 0, -1);
	geometry_triangle(geometry, 0, 3, 2);
	geometry_triangle(geometry, 0, 1, 3);

	geometry_position(geometry, -1, -1, 0);
	geometry_uvs(geometry, 0, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, 0);
	geometry_uvs(geometry, 0, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, 0);
	geometry_uvs(geometry, 1, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, 0);
	geometry_uvs(geometry, 1, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	// Face #4 (mirror of #3)

	geometry_normals(geometry, 0, 0, +1);
	geometry_triangle(geometry, 0, 3, 1);
	geometry_triangle(geometry, 0, 2, 3);

	geometry_position(geometry, -1, -1, 0);
	geometry_uvs(geometry, 1, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, 0);
	geometry_uvs(geometry, 1, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, 0);
	geometry_uvs(geometry, 0, 0);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, 0);
	geometry_uvs(geometry, 0, 1);
	geometry_position_apply(geometry, rotate);
	geometry_vertex(geometry);
}

void make_player(size_t index, unsigned short *indices, struct vertex_s *vertices, float x, float y, float z, float rx, float ry) {
    float ao[6][4] = {0};
    float light[6][4] = {
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8}
    };
    make_cube_faces(
        index, indices, vertices, ao, light,
        1, 1, 1, 1, 1, 1,
        226, 224, 241, 209, 225, 227,
        0, 0, 0, 0.4);
	mat4 ma = GLM_MAT4_IDENTITY;
	mat4 mb = GLM_MAT4_IDENTITY;
	glm_rotate_y(mb, rx, mb);
	glm_mul(mb, ma, ma);
	glm_rotate(mb, -ry, (vec3){ cosf(rx), 0, sinf(rx) });
	glm_mul(mb, ma, ma);
    mat_apply(data, ma, 36, 3, 10);
	glm_translate(mb, (vec3){ x, y, z });
	glm_mul(mb, ma, ma);
    mat_apply(data, ma, 36, 0, 10);
}

void make_cube_wireframe(float *data, float x, float y, float z, float n) {
    static const float positions[8][3] = {
        {-1, -1, -1},
        {-1, -1, +1},
        {-1, +1, -1},
        {-1, +1, +1},
        {+1, -1, -1},
        {+1, -1, +1},
        {+1, +1, -1},
        {+1, +1, +1}
    };
    static const int indices[24] = {
        0, 1, 0, 2, 0, 4, 1, 3,
        1, 5, 2, 3, 2, 6, 3, 7,
        4, 5, 4, 6, 5, 7, 6, 7
    };
    float *d = data;
    for (int i = 0; i < 24; i++) {
        int j = indices[i];
        *(d++) = x + n * positions[j][0];
        *(d++) = y + n * positions[j][1];
        *(d++) = z + n * positions[j][2];
    }
}

void make_character(
    float *data,
    float x, float y, float n, float m, char c)
{
    float *d = data;
    float s = 0.0625;
    float a = s;
    float b = s * 2;
    int w = c - 32;
    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y - m;
    *(d++) = du + a; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y + m;
    *(d++) = du + 0; *(d++) = dv + b;
}

void make_character_3d(
    float *data, float x, float y, float z, float n, int face, char c)
{
    static const float positions[8][6][3] = {
        {{0, -2, -1}, {0, +2, +1}, {0, +2, -1},
         {0, -2, -1}, {0, -2, +1}, {0, +2, +1}},
        {{0, -2, -1}, {0, +2, +1}, {0, -2, +1},
         {0, -2, -1}, {0, +2, -1}, {0, +2, +1}},
        {{-1, -2, 0}, {+1, +2, 0}, {+1, -2, 0},
         {-1, -2, 0}, {-1, +2, 0}, {+1, +2, 0}},
        {{-1, -2, 0}, {+1, -2, 0}, {+1, +2, 0},
         {-1, -2, 0}, {+1, +2, 0}, {-1, +2, 0}},
        {{-1, 0, +2}, {+1, 0, +2}, {+1, 0, -2},
         {-1, 0, +2}, {+1, 0, -2}, {-1, 0, -2}},
        {{-2, 0, +1}, {+2, 0, -1}, {-2, 0, -1},
         {-2, 0, +1}, {+2, 0, +1}, {+2, 0, -1}},
        {{+1, 0, +2}, {-1, 0, -2}, {-1, 0, +2},
         {+1, 0, +2}, {+1, 0, -2}, {-1, 0, -2}},
        {{+2, 0, -1}, {-2, 0, +1}, {+2, 0, +1},
         {+2, 0, -1}, {-2, 0, -1}, {-2, 0, +1}}
    };
    static const float uvs[8][6][2] = {
        {{0, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 1}},
        {{1, 0}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1}},
        {{1, 0}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}},
        {{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}},
        {{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}},
        {{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}}
    };
    static const float offsets[8][3] = {
        {-1, 0, 0}, {+1, 0, 0}, {0, 0, -1}, {0, 0, +1},
        {0, +1, 0}, {0, +1, 0}, {0, +1, 0}, {0, +1, 0},
    };
    float *d = data;
    float s = 0.0625;
    float pu = s / 5;
    float pv = s / 2.5;
    float u1 = pu;
    float v1 = pv;
    float u2 = s - pu;
    float v2 = s * 2 - pv;
    float p = 0.5;
    int w = c - 32;
    float du = (w % 16) * s;
    float dv = 1 - (w / 16 + 1) * s * 2;
    x += p * offsets[face][0];
    y += p * offsets[face][1];
    z += p * offsets[face][2];
    for (int i = 0; i < 6; i++) {
        *(d++) = x + n * positions[face][i][0];
        *(d++) = y + n * positions[face][i][1];
        *(d++) = z + n * positions[face][i][2];
        *(d++) = du + (uvs[face][i][0] ? u2 : u1);
        *(d++) = dv + (uvs[face][i][1] ? v2 : v1);
    }
}

int _make_sphere(
    float *data, float r, int detail,
    float *a, float *b, float *c,
    float *ta, float *tb, float *tc)
{
    if (detail == 0) {
        float *d = data;
        *(d++) = a[0] * r; *(d++) = a[1] * r; *(d++) = a[2] * r;
        *(d++) = a[0]; *(d++) = a[1]; *(d++) = a[2];
        *(d++) = ta[0]; *(d++) = ta[1];
        *(d++) = b[0] * r; *(d++) = b[1] * r; *(d++) = b[2] * r;
        *(d++) = b[0]; *(d++) = b[1]; *(d++) = b[2];
        *(d++) = tb[0]; *(d++) = tb[1];
        *(d++) = c[0] * r; *(d++) = c[1] * r; *(d++) = c[2] * r;
        *(d++) = c[0]; *(d++) = c[1]; *(d++) = c[2];
        *(d++) = tc[0]; *(d++) = tc[1];
        return 1;
    }
    else {
        vec3 ab, ac, bc;
        for (int i = 0; i < 3; i++) {
            ab[i] = (a[i] + b[i]) / 2;
            ac[i] = (a[i] + c[i]) / 2;
            bc[i] = (b[i] + c[i]) / 2;
        }
		glm_vec3_normalize(ab);
		glm_vec3_normalize(ac);
		glm_vec3_normalize(bc);
        vec2 tab, tac, tbc;
        tab[0] = 0; tab[1] = 1 - acosf(ab[1]) / PI;
        tac[0] = 0; tac[1] = 1 - acosf(ac[1]) / PI;
        tbc[0] = 0; tbc[1] = 1 - acosf(bc[1]) / PI;
        int total = 0;
        int n;
        n = _make_sphere(data, r, detail - 1, a, ab, ac, ta, tab, tac);
        total += n; data += n * 24;
        n = _make_sphere(data, r, detail - 1, b, bc, ab, tb, tbc, tab);
        total += n; data += n * 24;
        n = _make_sphere(data, r, detail - 1, c, ac, bc, tc, tac, tbc);
        total += n; data += n * 24;
        n = _make_sphere(data, r, detail - 1, ab, bc, ac, tab, tbc, tac);
        total += n; data += n * 24;
        return total;
    }
}

void make_sphere(float *data, float r, int detail) {
    // detail, triangles, floats
    // 0, 8, 192
    // 1, 32, 768
    // 2, 128, 3072
    // 3, 512, 12288
    // 4, 2048, 49152
    // 5, 8192, 196608
    // 6, 32768, 786432
    // 7, 131072, 3145728
    static int indices[8][3] = {
        {4, 3, 0}, {1, 4, 0},
        {3, 4, 5}, {4, 1, 5},
        {0, 3, 2}, {0, 2, 1},
        {5, 2, 3}, {5, 1, 2}
    };
    static float positions[6][3] = {
        { 0, 0,-1}, { 1, 0, 0},
        { 0,-1, 0}, {-1, 0, 0},
        { 0, 1, 0}, { 0, 0, 1}
    };
    static float uvs[6][3] = {
        {0, 0.5}, {0, 0.5},
        {0, 0}, {0, 0.5},
        {0, 1}, {0, 0.5}
    };
    int total = 0;
    for (int i = 0; i < 8; i++) {
        int n = _make_sphere(
            data, r, detail,
            positions[indices[i][0]],
            positions[indices[i][1]],
            positions[indices[i][2]],
            uvs[indices[i][0]],
            uvs[indices[i][1]],
            uvs[indices[i][2]]);
        total += n; data += n * 24;
    }
}
