//
//  mesh.c
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#include <Voxelworld/mesh.h>

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <Voxelworld/geometry.h>
#include <Voxelworld/gl.h>

struct mesh_s {
	/// Vertex Array Object
	GLuint vao;
	/// Vertex Buffer Object
	GLuint vbo;
	/// Entity Buffer Object
	GLuint ebo;
	/// Usage hint
	GLenum usage;
	/// What kind of primitives the mesh is composed of, like triangles or points
	GLenum primitive;
	/// How the indices are represented
	GLenum index;
	/// The number of primitives stored in this mesh
	GLsizei count;
};

//MARK: - Lifecycle Management

size_t mesh_sizeof() {
	return sizeof(struct mesh_s);
}

void mesh_init(mesh_t mesh, int32_t primitive, int32_t usage) {
	assert(mesh);
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(2, &mesh->vbo);
	mesh->count = 0;
	mesh->index = 0;
	mesh->usage = usage;
	mesh->primitive = primitive;
}

void mesh_deinit(mesh_t mesh) {
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(2, &mesh->vbo);
}

void mesh_destroy(mesh_t mesh) {
	mesh_deinit(mesh);
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

static void mesh_indices(mesh_t mesh, void *indices, size_t size) {
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
	glDebugClear;
	glBindVertexArray(mesh->vao);
	glDrawElements(mesh->primitive, mesh->count, mesh->index, 0);
	glBindVertexArray(0);
	glDebugCheck;
}

//MARK: - Geometry Integration

void mesh_update(mesh_t mesh, geometry_t geometry, int32_t primitive) {
	size_t size;
	
	void *vertices = geometry_vertices(geometry, &size);
	mesh_vertices(mesh, vertices, size);

	void *indices = geometry_indices(geometry, &size);
	mesh_indices(mesh, indices, size);

	// Update the primitive configuration
	mesh->count = (GLsizei)geometry_index_count(geometry);
	mesh->primitive = primitive;
	GLenum index[] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 0, GL_UNSIGNED_INT };
	mesh->index = index[geometry_index_size(geometry)];
}
