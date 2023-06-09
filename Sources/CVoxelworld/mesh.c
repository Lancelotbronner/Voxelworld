//
//  mesh.c
//  
//
//  Created by Christophe Bronner on 2023-06-04.
//

#include "mesh.h"

//MARK: - Mesh Management

mesh_t mesh_init() {
	mesh_t mesh = {};
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	glGenBuffers(2, &mesh.vbo);
	return mesh;
}

void mesh_delete(mesh_t *mesh) {
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(2, &mesh->vbo);
	*mesh = (mesh_t){ };
}

void mesh_update(mesh_t *mesh, void *vertices, GLsizei vertexCount, void *indices, GLsizei indexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vao);
	glBufferData(GL_ARRAY_BUFFER, vertexCount, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	mesh->size = indexCount;
}

void mesh_draw(const mesh_t *mesh) {
	if (!mesh->vao)
		return;
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->size);
}

//MARK: - Geometry Management

struct geometry_s {
	unsigned short *indices;
	struct vertex_s *vertices;
	unsigned short *index_buffer;
	struct vertex_s *vertex_buffer;
	struct vertex_s vertex;
	// Sizes are in faces
	size_t size;
	size_t capacity;
	float du, dv;
	float dx, dy, dz;
	unsigned short di;
};

geometry_t geometry_init(size_t capacity) {
	geometry_t geometry = malloc(sizeof(struct geometry_s));
	geometry->capacity = capacity;
	geometry->index_buffer = malloc(sizeof(unsigned short) * capacity * 6);
	geometry->indices = geometry->index_buffer;
	geometry->vertex_buffer = malloc(sizeof(struct vertex_s) * capacity * 4);
	geometry->vertices = geometry->vertex_buffer;
	return geometry;
}

void geometry_delete(geometry_t geometry) {
	free(geometry->index_buffer);
	free(geometry->vertex_buffer);
	free(geometry);
}

void geometry_normals_apply(geometry_t geometry, mat4 transform) {
	glm_mat4_mulv3(transform, geometry->vertex.normal, 0, geometry->vertex.normal);
}

void geometry_position_apply(geometry_t geometry,mat4 transform) {
	glm_mat4_mulv3(transform, geometry->vertex.position, 0, geometry->vertex.position);
}

void geometry_uvs_offset(geometry_t geometry, float du, float dv) {
	geometry->du = du;
	geometry->dv = dv;
}

void geometry_position_offset(geometry_t geometry, float dx, float dy, float dz) {
	geometry->dx = dx;
	geometry->dy = dy;
	geometry->dz = dz;
}

void geometry_position(geometry_t geometry, float x, float y, float z) {
	geometry->vertex.position[0] = geometry->dx + x;
	geometry->vertex.position[1] = geometry->dy + y;
	geometry->vertex.position[2] = geometry->dz + z;
}

void geometry_normals(geometry_t geometry, float x, float y, float z) {
	geometry->vertex.normal[0] = x;
	geometry->vertex.normal[1] = y;
	geometry->vertex.normal[2] = z;
}

void geometry_uvs(geometry_t geometry, float u, float v) {
	geometry->vertex.uv[0] = geometry->du + u / 16;
	geometry->vertex.uv[1] = geometry->dv + v / 16;
}

void geometry_lighting(geometry_t geometry, float ao, float light) {
	geometry->vertex.ao = ao;
	geometry->vertex.light = light;
}

void geometry_vertex(geometry_t geometry) {
	*geometry->vertices++ = geometry->vertex;
	geometry->di++;
}

void geometry_triangle(geometry_t geometry, unsigned short a, unsigned short b, unsigned short c) {
	*geometry->indices++ = geometry->di + a;
	*geometry->indices++ = geometry->di + b;
	*geometry->indices++ = geometry->di + c;
}
