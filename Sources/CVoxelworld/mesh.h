//
//  mesh.h
//  
//
//  Created by Christophe Bronner on 2023-06-04.
//

#pragma once

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

//MARK: - Struct Management

typedef struct mesh_s {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLsizei size;
} mesh_t;

mesh_t mesh_init();
void mesh_delete(mesh_t *mesh);

void mesh_update(mesh_t *mesh, void *vertices, GLsizei vertexCount, void *indices, GLsizei indexCount);
void mesh_draw(const mesh_t *mesh);

//MARK: - Vertex Management


struct vertex_s {
	float position[3];
	float normal[3];
	float uv[2];
	float ao;
	float light;
};

//MARK: - Geometry Management

//TODO: Specialized geometry to make mesh-building simpler?

typedef struct geometry_s *geometry_t;

geometry_t geometry_init(size_t capacity);
void geometry_delete(geometry_t geometry);

void geometry_triangle(geometry_t geometry, unsigned short a, unsigned short b, unsigned short c);

/// Applies the given transform to the current vertex's normals
void geometry_normals_apply(geometry_t geometry, mat4 transform);

/// Applies the given transform to the current vertex's position
void geometry_position_apply(geometry_t geometry, mat4 transform);

/// Applies the given offset to futur UVs
void geometry_uvs_offset(geometry_t geometry, float du, float dv);

/// Applies the given offset to futur positions
void geometry_position_offset(geometry_t geometry, float dx, float dy, float dz);

void geometry_position(geometry_t geometry, float x, float y, float z);

void geometry_normals(geometry_t geometry, float x, float y, float z);

void geometry_uvs(geometry_t geometry, float u, float v);

void geometry_lighting(geometry_t geometry, float ao, float light);

/// Commits the current vertex to the geometry
void geometry_vertex(geometry_t geometry);
