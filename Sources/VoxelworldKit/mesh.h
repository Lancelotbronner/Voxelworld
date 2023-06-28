//
//  mesh.h
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#pragma once

#include <Voxelworld/mesh.h>

#include <OpenGL/gl3.h>

struct mesh_s {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLenum usage;
	GLenum primitive;
	GLenum index;
	GLsizei count;
};

void mesh_indices(mesh_t mesh, void *indices, size_t size);
