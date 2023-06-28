//
//  mesh+geometry.c
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#include <Voxelworld/mesh+geometry.h>

#include "geometry.h"
#include "mesh.h"

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
	mesh->index = 0;
	if (geometry->index == 1)
		mesh->index = GL_UNSIGNED_BYTE;
	else if (geometry->index == 2)
		mesh->index = GL_UNSIGNED_SHORT;
	else if (geometry->index == 4)
		mesh->index = GL_UNSIGNED_INT;
}
