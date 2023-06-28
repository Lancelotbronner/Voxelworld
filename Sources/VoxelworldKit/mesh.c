//
//  mesh.c
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#include "mesh.h"

#include <stdlib.h>
#include <OpenGL/gl3.h>

//MARK: - Lifecycle Management

mesh_t mesh_init(int32_t primitive, int32_t usage) {
	mesh_t mesh = malloc(sizeof(struct mesh_s));
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(2, &mesh->vbo);
	mesh->count = 0;
	mesh->usage = usage;
	mesh->primitive = primitive;
}

void mesh_destroy(mesh_t mesh) {
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(2, &mesh->vbo);
	free(mesh);
}

//MARK: - Binding Management

void mesh_bind(mesh_t mesh) {
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
}

void mesh_unbind() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//MARK: - Content Management

void mesh_vertices(mesh_t mesh, void *vertices, size_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, mesh->usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh_indices(mesh_t mesh, void *indices, size_t size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, mesh->usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void mesh_indices8(mesh_t mesh, unsigned char *indices, size_t count) {
	mesh_indices(mesh, indices, count);
	mesh->index = GL_UNSIGNED_BYTE;
}

void mesh_indices16(mesh_t mesh, unsigned short *indices, size_t count) {
	mesh_indices(mesh, indices, count * 2);
	mesh->index = GL_UNSIGNED_SHORT;
}

void mesh_indices32(mesh_t mesh, unsigned int *indices, size_t count) {
	mesh_indices(mesh, indices, count * 4);
	mesh->index = GL_UNSIGNED_INT;
}

//MARK: - Drawing Management

void mesh_draw(mesh_t mesh) {
	glBindVertexArray(mesh->vao);
	glDrawElements(mesh->primitive, mesh->count, mesh->index, 0);
	glBindVertexArray(0);
}
