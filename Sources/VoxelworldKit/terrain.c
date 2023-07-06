//
//  terrain.c
//  
//
//  Created by Christophe Bronner on 2023-06-29.
//

#include <Voxelworld/terrain.h>

#include <cglm/cglm.h>
#include <OpenGL/gl3.h>
#include <Voxelworld/mesh+geometry.h>
#include "terrain.h"

//MARK: - Terrain

/// Clears the current terrain geometry.
void terrain_clear() {
	if (terrain.initialized) {
		geometry_clear(&terrain.geometry);
		return;
	}
	geometry_init(&terrain.geometry, 100000, sizeof(struct terrain_vertex_s), 150000, sizeof(unsigned int));
	terrain.initialized = true;
}

/// Uploads the current terrain to a mesh.
/// - Parameter mesh: The mesh to update with the geometry.
void terrain_upload(mesh_t mesh) {
	mesh_update(mesh, &terrain.geometry, GL_TRIANGLES);
}

//MARK: - Terrain Geometry Management

void terrain_cross(int texture, float ao, float light, float x, float y, float z, float rotation) {
	// Configure the block information
	calculate_uvs(texture);
	terrain_position_offset(x, y, z);
	terrain_lighting(ao, light);

	// Prepare rotation
	mat4 rotate = GLM_MAT4_IDENTITY;
	glm_rotate_y(rotate, rotation * CGLM_PI / 180, rotate);
	terrain_position_transform(rotate);

	// Face #1

	terrain_normals(-1, 0, 0);
	terrain_triangle(0, 3, 2);
	terrain_triangle(0, 1, 3);

	terrain_position(0, -1, -1);
	terrain_uvs(0, 0);
	terrain_vertex();

	terrain_position(0, -1, +1);
	terrain_uvs(1, 0);
	terrain_vertex();

	terrain_position(0, +1, -1);
	terrain_uvs(0, 1);
	terrain_vertex();

	terrain_position(0, +1, +1);
	terrain_uvs(1, 1);
	terrain_vertex();

	// Face #2 (mirror of #1)

	terrain_normals(+1, 0, 0);
	terrain_triangle(0, 3, 1);
	terrain_triangle(0, 2, 3);

	terrain_position(0, -1, -1);
	terrain_uvs(1, 0);
	terrain_vertex();

	terrain_position(0, -1, +1);
	terrain_uvs(0, 0);
	terrain_vertex();

	terrain_position(0, +1, -1);
	terrain_uvs(1, 1);
	terrain_vertex();

	terrain_position(0, +1, +1);
	terrain_uvs(0, 1);
	terrain_vertex();

	// Face #3

	terrain_normals(0, 0, -1);
	terrain_triangle(0, 3, 2);
	terrain_triangle(0, 1, 3);

	terrain_position(-1, -1, 0);
	terrain_uvs(0, 0);
	terrain_vertex();

	terrain_position(-1, +1, 0);
	terrain_uvs(0, 1);
	terrain_vertex();

	terrain_position(+1, -1, 0);
	terrain_uvs(1, 0);
	terrain_vertex();

	terrain_position(+1, +1, 0);
	terrain_uvs(1, 1);
	terrain_vertex();

	// Face #4 (mirror of #3)

	terrain_normals(0, 0, +1);
	terrain_triangle(0, 3, 1);
	terrain_triangle(0, 2, 3);

	terrain_position(-1, -1, 0);
	terrain_uvs(1, 0);
	terrain_vertex();

	terrain_position(-1, +1, 0);
	terrain_uvs(1, 1);
	terrain_vertex();

	terrain_position(+1, -1, 0);
	terrain_uvs(0, 0);
	terrain_vertex();

	terrain_position(+1, +1, 0);
	terrain_uvs(0, 1);
	terrain_vertex();

	// cleanup
	terrain_position_identity();
}

void terrain_cube(int textures[6], float ao[6][4], float light[6][4], unsigned char faces, float x, float y, float z) {
	// Configure the geometry for the block
	terrain_position_offset(x, y, z);

	if (faces & 0x1) {
		calculate_uvs(textures[0]);
		terrain_west(ao[0], light[0]);
	}

	if (faces & 0x2) {
		calculate_uvs(textures[1]);
		terrain_east(ao[1], light[1]);
	}

	if (faces & 0x4) {
		calculate_uvs(textures[2]);
		terrain_top(ao[2], light[2]);
	}

	if (faces & 0x8) {
		calculate_uvs(textures[3]);
		terrain_bottom(ao[3], light[3]);
	}

	if (faces & 0x10) {
		calculate_uvs(textures[4]);
		terrain_north(ao[4], light[4]);
	}

	if (faces & 0x20) {
		calculate_uvs(textures[5]);
		terrain_south(ao[5], light[5]);
	}
}
