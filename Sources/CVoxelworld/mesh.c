//
//  mesh.c
//  
//
//  Created by Christophe Bronner on 2023-06-04.
//

#include "mesh.h"

//MARK: - Mesh Management

legacy_mesh_t mesh_open() {
	legacy_mesh_t mesh = {};
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	glGenBuffers(2, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	return mesh;
}

void mesh_close() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void mesh_delete(legacy_mesh_t *mesh) {
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(2, &mesh->vbo);
	*mesh = (legacy_mesh_t){ };
}

void mesh_legacy_update(legacy_mesh_t *mesh, void *vertices, GLsizei vertexCount, void *indices, GLsizei indexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	mesh->size = indexCount;
}

void mesh_legacy_draw(const legacy_mesh_t *mesh) {
	if (!mesh->vao)
		return;
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

//MARK: - Geometry Management

struct geometry_s {
	unsigned short *indices;
	struct vertex_s *vertices;
	unsigned short *index_buffer;
	struct vertex_s *vertex_buffer;
	mat4 position_modifier;
	mat4 normal_modifier;
	struct vertex_s vertex;
	GLsizei capacity;
	float du, dv;
	float dx, dy, dz;
	unsigned short di;
	bool position_modifier_enabled;
	bool normal_modifier_enabled;
};

geometry_t geometry_legacy_init(GLsizei capacity) {
	geometry_t geometry = malloc(sizeof(struct geometry_s));
	geometry->capacity = capacity;
	geometry->index_buffer = malloc(sizeof(unsigned short) * capacity * 6);
	geometry->indices = geometry->index_buffer;
	geometry->vertex_buffer = malloc(sizeof(struct vertex_s) * capacity * 4);
	geometry->vertices = geometry->vertex_buffer;
	return geometry;
}

void geometry_legacy_upload(geometry_t geometry, GLuint vbo, GLuint ebo, GLsizei *size) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	size_t vertices = geometry->vertices - geometry->vertex_buffer;
	glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(struct vertex_s), geometry->vertex_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	size_t indices = geometry->indices - geometry->index_buffer;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(unsigned short), geometry->index_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	*size = (GLsizei)indices;
}

void geometry_upload_to(geometry_t geometry, legacy_mesh_t *mesh) {
	geometry_legacy_upload(geometry, mesh->vbo, mesh->ebo, &mesh->size);
	// reset the geometry
	struct geometry_s tmp = *geometry;
	*geometry = (struct geometry_s){ };
	geometry->capacity = tmp.capacity;
	geometry->indices = geometry->index_buffer = tmp.index_buffer;
	geometry->vertices = geometry->vertex_buffer = tmp.vertex_buffer;
}

void geometry_legacy_delete(geometry_t geometry) {
	free(geometry->index_buffer);
	free(geometry->vertex_buffer);
	free(geometry);
}

void geometry_normal_apply(geometry_t geometry, mat4 transform) {
	glm_mat4_mulv3(transform, geometry->vertex.normal, 0, geometry->vertex.normal);
}

void geometry_normal_enable(geometry_t geometry, mat4 transform) {
	glm_mat4_copy(transform, geometry->normal_modifier);
	geometry->normal_modifier_enabled = true;
}

void geometry_normal_disable(geometry_t geometry) {
	geometry->normal_modifier_enabled = false;
}

void geometry_position_apply(geometry_t geometry, mat4 transform) {
	glm_mat4_mulv3(transform, geometry->vertex.position, 0, geometry->vertex.position);
}

void geometry_position_enable(geometry_t geometry, mat4 transform) {
	glm_mat4_copy(transform, geometry->position_modifier);
	geometry->position_modifier_enabled = true;
}

void geometry_position_disable(geometry_t geometry) {
	geometry->position_modifier_enabled = false;
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
	geometry->vertex.position[0] = x / 2;
	geometry->vertex.position[1] = y / 2;
	geometry->vertex.position[2] = z / 2;
	if (geometry->position_modifier_enabled)
		glm_mat4_mulv3(geometry->position_modifier, geometry->vertex.position, 0, geometry->vertex.position);
	geometry->vertex.position[0] += geometry->dx;
	geometry->vertex.position[1] += geometry->dy;
	geometry->vertex.position[2] += geometry->dz;
}

void geometry_normals(geometry_t geometry, float x, float y, float z) {
	geometry->vertex.normal[0] = x;
	geometry->vertex.normal[1] = y;
	geometry->vertex.normal[2] = z;
	if (geometry->normal_modifier_enabled)
		glm_mat4_mulv3(geometry->normal_modifier, geometry->vertex.normal, 0, geometry->vertex.normal);
}

void geometry_uvs(geometry_t geometry, float u, float v) {
	geometry->vertex.uv[0] = geometry->du + u / 16;
	geometry->vertex.uv[1] = geometry->dv + v / 16;
}

void geometry_lighting(geometry_t geometry, float ao, float light) {
	geometry->vertex.ao = ao;
	geometry->vertex.light = light;
}

void geometry_legacy_vertex(geometry_t geometry) {
	*geometry->vertices++ = geometry->vertex;
	geometry->di++;
}

void geometry_triangle(geometry_t geometry, unsigned short a, unsigned short b, unsigned short c) {
	*geometry->indices++ = geometry->di + a;
	*geometry->indices++ = geometry->di + b;
	*geometry->indices++ = geometry->di + c;
}
