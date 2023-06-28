//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#pragma once

#include <stdint.h>

//MARK: - Structures

typedef struct mesh_s *mesh_t;

//MARK: - Lifecycle Management

mesh_t mesh_init(int32_t primitive, int32_t usage);
void mesh_destroy(mesh_t mesh);

//MARK: - Binding Management

void mesh_bind(mesh_t mesh);
void mesh_unbind();

//MARK: - Content Management

void mesh_vertices(mesh_t mesh, void *vertices, size_t size);
void mesh_indices8(mesh_t mesh, unsigned char *indices, size_t count);
void mesh_indices16(mesh_t mesh, unsigned short *indices, size_t count);
void mesh_indices32(mesh_t mesh, unsigned int *indices, size_t count);

//MARK: - Drawing Management

void mesh_draw(mesh_t mesh);
