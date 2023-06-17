#include <math.h>
#include "cube.h"
#include "item.h"
#include "matrix.h"
#include "utils.h"
#include "mesh.h"

//MARK: - Utilities

static const float S = 1.0 / 16.0;
//static const float F = 1.0 / 2048.0;
//static const float T = S - F;
static const float F = 0; // 1.0 / 2048.0;
static const float T = 1; // S - F;

static inline void calculate_uvs(int w, float *du, float *dv) {
	*du = w % 16 * S;
	*dv = w / 16 * S;
}

//MARK: - Cube Geometry

void generate_cube_geometry(geometry_t geometry, int id[6], float ao[6][4], float light[6][4], int face[6], float x, float y, float z) {
	float du, dv;

	// Configure the geometry for the block
	geometry_position_offset(geometry, x, y, z);

	if (face[0]) {
		calculate_uvs(id[0], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_west_face(geometry, ao[0], light[0]);
	}

	if (face[1]) {
		calculate_uvs(id[1], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_east_face(geometry, ao[1], light[1]);
	}

	if (face[2]) {
		calculate_uvs(id[2], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_top_face(geometry, ao[2], light[2]);
	}

	if (face[3]) {
		calculate_uvs(id[3], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_bottom_face(geometry, ao[3], light[3]);
	}

	if (face[4]) {
		calculate_uvs(id[4], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_north_face(geometry, ao[4], light[4]);
	}

	if (face[5]) {
		calculate_uvs(id[5], &du, &dv);
		geometry_uvs_offset(geometry, du, dv);
		generate_south_face(geometry, ao[5], light[5]);
	}
}

void generate_north_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, 0, 0, -1);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 1, 2);
		geometry_triangle(geometry, 1, 3, 2);
	} else {
		geometry_triangle(geometry, 0, 3, 2);
		geometry_triangle(geometry, 0, 1, 3);
	}

	// Insert vertices

	geometry_position(geometry, -1, -1, -1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, -1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, -1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, -1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

void generate_south_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, 0, 0, +1);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 2, 1);
		geometry_triangle(geometry, 2, 3, 1);
	} else {
		geometry_triangle(geometry, 0, 3, 1);
		geometry_triangle(geometry, 0, 2, 3);
	}

	// Insert vertices

	geometry_position(geometry, -1, -1, +1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, +1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, +1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, +1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

void generate_west_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, -1, 0, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 1, 2);
		geometry_triangle(geometry, 1, 3, 2);
	} else {
		geometry_triangle(geometry, 0, 3, 2);
		geometry_triangle(geometry, 0, 1, 3);
	}

	// Insert vertices

	geometry_position(geometry, -1, -1, -1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, -1, +1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, -1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, +1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

void generate_east_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, +1, 0, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 2, 1);
		geometry_triangle(geometry, 2, 3, 1);
	} else {
		geometry_triangle(geometry, 0, 3, 1);
		geometry_triangle(geometry, 0, 2, 3);
	}
	
	// Insert vertices

	geometry_position(geometry, +1, -1, -1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, +1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, -1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, +1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

void generate_bottom_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, 0, -1, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 2, 1);
		geometry_triangle(geometry, 2, 3, 1);
	} else {
		geometry_triangle(geometry, 0, 3, 1);
		geometry_triangle(geometry, 0, 2, 3);
	}

	// Insert vertices

	geometry_position(geometry, -1, -1, -1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, -1, +1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, -1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, +1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

void generate_top_face(geometry_t geometry, float ao[4], float light[4]) {
	// Configure the geometry for the face
	geometry_normals(geometry, 0, +1, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		geometry_triangle(geometry, 0, 1, 2);
		geometry_triangle(geometry, 1, 3, 2);
	} else {
		geometry_triangle(geometry, 0, 3, 2);
		geometry_triangle(geometry, 0, 1, 3);
	}

	// Insert vertices

	geometry_position(geometry, -1, +1, -1);
	geometry_uvs(geometry, F, T);
	geometry_lighting(geometry, ao[0], light[0]);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, +1);
	geometry_uvs(geometry, F, F);
	geometry_lighting(geometry, ao[1], light[1]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, -1);
	geometry_uvs(geometry, T, T);
	geometry_lighting(geometry, ao[2], light[2]);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, +1);
	geometry_uvs(geometry, T, F);
	geometry_lighting(geometry, ao[3], light[3]);
	geometry_vertex(geometry);
}

//MARK: - Cross Geometry

void generate_cross_geometry(geometry_t geometry, int id, float ao, float light, float x, float y, float z, float rotation) {
	// Calculate the UV offset
	float du, dv;
	calculate_uvs(plants[id], &du, &dv);
	geometry_uvs_offset(geometry, du, dv);

	// Configure the block information
	geometry_position_offset(geometry, x, y, z);
	geometry_lighting(geometry, ao, light);

	// Prepare normals rotation
	mat4 rotate = GLM_MAT4_IDENTITY;
	glm_rotate_y(rotate, RADIANS(rotation), rotate);
	geometry_position_enable(geometry, rotate);

	// Face #1

	geometry_normals(geometry, -1, 0, 0);
	geometry_triangle(geometry, 0, 3, 2);
	geometry_triangle(geometry, 0, 1, 3);

	geometry_position(geometry, 0, -1, -1);
	geometry_uvs(geometry, 0, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, -1, +1);
	geometry_uvs(geometry, 1, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, -1);
	geometry_uvs(geometry, 0, 1);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, +1);
	geometry_uvs(geometry, 1, 1);
	geometry_vertex(geometry);

	// Face #2 (mirror of #1)

	geometry_normals(geometry, +1, 0, 0);
	geometry_triangle(geometry, 0, 3, 1);
	geometry_triangle(geometry, 0, 2, 3);

	geometry_position(geometry, 0, -1, -1);
	geometry_uvs(geometry, 1, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, -1, +1);
	geometry_uvs(geometry, 0, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, -1);
	geometry_uvs(geometry, 1, 1);
	geometry_vertex(geometry);

	geometry_position(geometry, 0, +1, +1);
	geometry_uvs(geometry, 0, 1);
	geometry_vertex(geometry);

	// Face #3

	geometry_normals(geometry, 0, 0, -1);
	geometry_triangle(geometry, 0, 3, 2);
	geometry_triangle(geometry, 0, 1, 3);

	geometry_position(geometry, -1, -1, 0);
	geometry_uvs(geometry, 0, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, 0);
	geometry_uvs(geometry, 0, 1);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, 0);
	geometry_uvs(geometry, 1, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, 0);
	geometry_uvs(geometry, 1, 1);
	geometry_vertex(geometry);

	// Face #4 (mirror of #3)

	geometry_normals(geometry, 0, 0, +1);
	geometry_triangle(geometry, 0, 3, 1);
	geometry_triangle(geometry, 0, 2, 3);

	geometry_position(geometry, -1, -1, 0);
	geometry_uvs(geometry, 1, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, -1, +1, 0);
	geometry_uvs(geometry, 1, 1);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, -1, 0);
	geometry_uvs(geometry, 0, 0);
	geometry_vertex(geometry);

	geometry_position(geometry, +1, +1, 0);
	geometry_uvs(geometry, 0, 1);
	geometry_vertex(geometry);

	// cleanup
	geometry_position_disable(geometry);
}

//MARK: - Character Geometry

void generate_player_geometry(geometry_t geometry, float x, float y, float z, float rx, float ry) {
	float du, dv;

	// Prepare the transforms and constants
	mat4 transform = GLM_MAT4_IDENTITY;
	glm_rotate(transform, rx, (vec3){ 0, 1, 0 });
	glm_rotate(transform, -ry, (vec3){ cosf(rx), 0, sinf(rx) });
	float ao[4] = { 0 };
	float light[4] = { 0.8, 0.8, 0.8, 0.8};

	// Configure the geometry context
	geometry_position_offset(geometry, x * 0.4, y * 0.4, z * 0.4);
	geometry_normal_enable(geometry, transform);

	// West Face
	calculate_uvs(226, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_west_face(geometry, ao, light);

	// East Face
	calculate_uvs(224, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_east_face(geometry, ao, light);

	// Top Face
	calculate_uvs(241, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_top_face(geometry, ao, light);

	// Bottom Face
	calculate_uvs(209, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_bottom_face(geometry, ao, light);

	// North Face
	calculate_uvs(225, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_north_face(geometry, ao, light);

	// South Face
	calculate_uvs(227, &du, &dv);
	geometry_uvs_offset(geometry, du, dv);
	generate_south_face(geometry, ao, light);

	// Cleanup the geometry context
	geometry_normal_disable(geometry);
}

//MARK: - Legacy Geometries

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
