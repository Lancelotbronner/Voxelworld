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

typedef struct legacy_mesh_s {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	/// How many indices are in this mesh
	GLsizei size;
} legacy_mesh_t;

legacy_mesh_t mesh_open();
void mesh_close();
void mesh_delete(legacy_mesh_t *mesh);

void mesh_legacy_update(legacy_mesh_t *mesh, void *vertices, GLsizei vertexCount, void *indices, GLsizei indexCount);
void mesh_legacy_draw(const legacy_mesh_t *mesh);

//MARK: - Vertex Management

struct vertex_s {
	float position[3];
	float normal[3];
	float uv[2];
	float ao;
	float light;
};

//MARK: - Geometry Management

typedef struct legacy_geometry_s *legacy_geometry_t;

/// Initializes an empty geometry with the specified capacity.
/// - Parameter capacity: The capacity in faces.
legacy_geometry_t geometry_legacy_init(GLsizei capacity);

void geometry_legacy_delete(legacy_geometry_t geometry);

void geometry_legacy_upload(legacy_geometry_t geometry, GLuint vbo, GLuint ebo, GLsizei *size);

void geometry_upload_to(legacy_geometry_t geometry, legacy_mesh_t *mesh);

// The following methods are specialized to the vertex structure above.

void geometry_triangle(legacy_geometry_t geometry, unsigned short a, unsigned short b, unsigned short c);

/// Applies the given transformation to the current vertex's normals
void geometry_normal_apply(legacy_geometry_t geometry, mat4 transform);

/// Enables the given transform to futur vertex normals
void geometry_normal_enable(legacy_geometry_t geometry, mat4 transform);

/// Disables the current normals transformation
void geometry_normal_disable(legacy_geometry_t geometry);

/// Applies the given transform to the current vertex's position
void geometry_position_apply(legacy_geometry_t geometry, mat4 transform);

/// Enables the given transform to futur vertex positions
void geometry_position_enable(legacy_geometry_t geometry, mat4 transform);

/// Disables the current position transformation
void geometry_position_disable(legacy_geometry_t geometry);

/// Applies the given offset to futur UVs
void geometry_uvs_offset(legacy_geometry_t geometry, float du, float dv);

/// Applies the given offset to futur positions
void geometry_position_offset(legacy_geometry_t geometry, float dx, float dy, float dz);

void geometry_position(legacy_geometry_t geometry, float x, float y, float z);

void geometry_normals(legacy_geometry_t geometry, float x, float y, float z);

void geometry_uvs(legacy_geometry_t geometry, float u, float v);

void geometry_lighting(legacy_geometry_t geometry, float ao, float light);

/// Commits the current vertex to the geometry
void geometry_legacy_vertex(legacy_geometry_t geometry);
