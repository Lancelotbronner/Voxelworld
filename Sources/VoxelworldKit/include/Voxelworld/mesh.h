//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#pragma once

#include <Voxelworld/types.h>

//MARK: - Lifecycle Management

size_t mesh_sizeof();

void mesh_init(mesh_t mesh, int32_t primitive, int32_t usage);

/// Uninitializes the mesh without deallocating it, allowing it to be reused.
/// - Parameter mesh: The mesh to prepare for reuse.
void mesh_deinit(mesh_t mesh);

void mesh_destroy(mesh_t mesh);

//MARK: - Binding Management

void mesh_bind(mesh_t mesh);
void mesh_unbind();

//MARK: - Content Management

void mesh_vertices(mesh_t mesh, void *vertices, size_t size);
void mesh_indices8(mesh_t mesh, unsigned char *indices, size_t count);
void mesh_indices16(mesh_t mesh, unsigned short *indices, size_t count);
void mesh_indices32(mesh_t mesh, unsigned int *indices, size_t count);

/// Updates an existing mesh with geometry.
/// - Parameters:
///   - mesh: The mesh to populate.
///   - geometry: The geometry to upload.
///   - primitive: The primitive this geometry represents.
void mesh_update(mesh_t mesh, geometry_t geometry, int32_t primitive);

//MARK: - Drawing Management

void mesh_draw(mesh_t mesh);
