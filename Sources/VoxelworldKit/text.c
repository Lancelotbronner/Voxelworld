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
	geometry_t geometry = text.geometry;
	unsigned int tmp[] = { text.di + a, text.di + b, text.di + c };
	geometry_index(text.geometry, tmp, 3 * sizeof(unsigned int));
}

void text_uvs_offset(float du, float dv) {
	text.du = du;
	text.dv = dv;
}

void text_uvs(float u, float v) {
	// multiply by the offset of a single char (32x64 px/char, 16x16 512px texture)
	text.vertex.uv[0] = u / 16.0f;
	text.vertex.uv[1] = v / 8.0f;
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
	geometry_t geometry = text.geometry;
	struct text_vertex_s *vertex = &text.vertex;
	vertex->position[0] += text.dx;
	vertex->position[1] += text.dy;
	vertex->uv[0] += text.du;
	vertex->uv[1] += text.dv;
	text.di++;
	geometry_vertex(text.geometry, vertex, sizeof(struct text_vertex_s));
}

//MARK: - Geometry Management
#include <stdio.h>
void text_character(char character, float size, float x, float y) {
	character -= 0x20;
	float u = character % 16 / 16.0f;
	float v = character / 16 / 16.0f;
	text_uvs_offset(u, v);
	text_position_offset(x, y);
	text_size(size);
	printf("X %f, Y %f, U %f, V %f\n", x, y, u * 16, v * 16);

	// Triangles

	text_triangle(0, 1, 3);
	text_triangle(1, 2, 3);

	// Vertices

	float s = size;

	text_position(0, 0);
	text_uvs(0, 0);
	text_vertex();

	text_position(s, 0);
	text_uvs(1, 0);
	text_vertex();

	text_position(s, s);
	text_uvs(1, 1);
	text_vertex();

	text_position(0, s);
	text_uvs(0, 1);
	text_vertex();
}

void text_string(char *characters, size_t length, float size, float x, float y) {
	for (int i = 0; i < length; i++) {
		switch (characters[i]) {
			case ' ':
				x += size;
				continue;
			case '\n':
				y += size;
				continue;
			default:
				text_character(characters[i], size, x, y);
				x += size;
		}
	}
}
