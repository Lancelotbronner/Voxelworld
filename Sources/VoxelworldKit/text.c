//
//  text.c
//  
//
//  Created by Christophe Bronner on 2023-07-06.
//

#include <Voxelworld/text.h>
#include <Voxelworld/text+vertex.h>

#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <Voxelworld/geometry.h>
#include <Voxelworld/mesh.h>

//MARK: - Vertex Management

void vertex_text() {
	GLsizei stride = sizeof(GLfloat) * 4;
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, sizeof(GLfloat) * 2);
}

//MARK: - Text Management

struct text_s {
	geometry_t geometry;
	struct text_vertex_s vertex;
	float du, dv;
	float dx, dy;
	float size;
	unsigned int di;
	bool initialized;
};

static thread_local struct text_s text;

void text_clear() {
	if (text.initialized) {
		geometry_clear(text.geometry);
		return;
	}
	text.geometry = malloc(geometry_sizeof());
	geometry_init(text.geometry, 16'384, sizeof(struct text_vertex_s), 24'576, sizeof(unsigned int));
	text.initialized = true;
}

void text_upload(mesh_t mesh) {
	mesh_update(mesh, text.geometry, GL_TRIANGLES);
}

//MARK: - Primitives Management

void text_triangle(unsigned int a, unsigned int b, unsigned int c) {
	unsigned int tmp[] = { text.di + a, text.di + b, text.di + c };
	text.di += 3;
	geometry_index(text.geometry, tmp, 3 * sizeof(unsigned int));
}

void text_uvs_offset(float du, float dv) {
	text.du = du;
	text.dv = dv;
}

void text_uvs(float u, float v) {
	text.vertex.uv[0] = u;
	text.vertex.uv[1] = v;
}

void text_size(float size) {
	text.size = size;
}

void text_position_offset(float dx, float dy) {
	text.dx = dx;
	text.dy = dy;
}

void text_position(float x, float y) {
	text.vertex.position[0] = x;
	text.vertex.position[1] = y;
}

void text_vertex() {
	text.vertex.position[0] += text.dx;
	text.vertex.position[1] += text.dy;
	text.vertex.uv[0] += text.du;
	text.vertex.uv[1] += text.dv;
	geometry_vertex(text.geometry, &text.vertex, sizeof(struct text_vertex_s));
}

//MARK: - Geometry Management

static const float F = 0.0f;
static const float T = 1.0f / 16.0f;

void text_character(char character, float size, float x, float y) {
	float u = character % 16 / 16.0f;
	float v = character / 16 / 16.0f;
	text_uvs_offset(u, v);
//	text_position_offset(x, y);
	text_position_offset(0, 0);
	text_size(size);

	// Triangles

	text_triangle(0, 1, 3);
	text_triangle(1, 2, 3);

	// Vertices

	float s = text.size;

	text_position(-s / 2, -s);
	text_uvs(F, F);
	text_vertex();

	text_position(s / 2, -s);
	text_uvs(T, F);
	text_vertex();

	text_position(s / 2, s);
	text_uvs(T, T);
	text_vertex();

	text_position(-s / 2, s);
	text_uvs(F, T);
	text_vertex();

	/*
	 float *d = data;
	 float s = 0.0625;
	 float a = s;
	 float b = s * 2;
	 int w = c - 32;
	 float du = (w % 16) * a;
	 float dv = 1 - (w / 16) * b - b;

	 //-n-m 0
	 *(d++) = x - n; *(d++) = y - m;
	 *(d++) = du + 0; *(d++) = dv;

	 //+n-m 1
	 *(d++) = x + n; *(d++) = y - m;
	 *(d++) = du + a; *(d++) = dv;

	 //+n+m 2
	 *(d++) = x + n; *(d++) = y + m;
	 *(d++) = du + a; *(d++) = dv + b;

	 //-n-m 0
	 *(d++) = x - n; *(d++) = y - m;
	 *(d++) = du + 0; *(d++) = dv;

	 // +n+m 2
	 *(d++) = x + n; *(d++) = y + m;
	 *(d++) = du + a; *(d++) = dv + b;

	 // -n+m 3
	 *(d++) = x - n; *(d++) = y + m;
	 *(d++) = du + 0; *(d++) = dv + b;
	 */
}

void text_string(char *characters, size_t length, float size, float x, float y) {
	for (int i = 0; i < length; i++) {
		text_character(characters[i], size, x, y);
		x += size;
	}
}
