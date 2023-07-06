//
//  terrain+geometry.c
//  
//
//  Created by Christophe Bronner on 2023-06-29.
//

#include <Voxelworld/terrain+geometry.h>

#include <cglm/mat4.h>
#include <OpenGL/gl3.h>
#include "terrain.h"

//MARK: - Vertex Management

void vertex_terrain() {
	GLsizei stride = sizeof(GLfloat) * 10;
	// position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// normal
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, sizeof(GLfloat) * 3);
	// uv light ao
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, sizeof(GLfloat) * 6);
}

//MARK: - Primitives Management

void terrain_triangle(unsigned int a, unsigned int b, unsigned int c) {
	unsigned int tmp[] = { a, b, c };
	geometry_index(&terrain.geometry, tmp, 3 * sizeof(unsigned int));
}

void terrain_uvs_offset(float du, float dv) {
	terrain.du = du;
	terrain.dv = dv;
}

void terrain_position_offset(float dx, float dy, float dz) {
	terrain.dx = dx;
	terrain.dy = dy;
	terrain.dz = dz;
}

void terrain_position(float x, float y, float z) {
	terrain.vertex.position[0] = x / 2;
	terrain.vertex.position[1] = y / 2;
	terrain.vertex.position[2] = z / 2;
}

void terrain_normals(float x, float y, float z) {
	terrain.vertex.normal[0] = x;
	terrain.vertex.normal[1] = y;
	terrain.vertex.normal[2] = z;
}

void terrain_uvs(float u, float v) {
	terrain.vertex.uv[0] = u;
	terrain.vertex.uv[1] = v;
}

void terrain_lighting(float ao, float light) {
	terrain.vertex.ao = ao;
	terrain.vertex.light = light;
}

void terrain_vertex() {
	if (terrain.flags & 0x1)
		glm_mat4_mulv3(terrain.position_transform, terrain.vertex.position, 0, terrain.vertex.position);
	terrain.vertex.position[0] += terrain.dx;
	terrain.vertex.position[1] += terrain.dy;
	terrain.vertex.position[2] += terrain.dz;
	terrain.vertex.uv[0] += terrain.du;
	terrain.vertex.uv[1] += terrain.dv;
	geometry_vertex(&terrain.geometry, &terrain.vertex, sizeof(struct terrain_vertex_s));
}

//MARK: - Geometry Management

void terrain_bottom(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(0, -1, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 2, 1);
		terrain_triangle(2, 3, 1);
	} else {
		terrain_triangle(0, 3, 1);
		terrain_triangle(0, 2, 3);
	}

	// Insert vertices

	terrain_position(-1, -1, -1);
	terrain_uvs(F, F);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(-1, -1, +1);
	terrain_uvs(F, T);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(+1, -1, -1);
	terrain_uvs(T, F);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(+1, -1, +1);
	terrain_uvs(T, T);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}

void terrain_top(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(0, +1, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 1, 2);
		terrain_triangle(1, 3, 2);
	} else {
		terrain_triangle(0, 3, 2);
		terrain_triangle(0, 1, 3);
	}

	// Insert vertices

	terrain_position(-1, +1, -1);
	terrain_uvs(F, T);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(-1, +1, +1);
	terrain_uvs(F, F);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(+1, +1, -1);
	terrain_uvs(T, T);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(+1, +1, +1);
	terrain_uvs(T, F);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}

void terrain_west(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(-1, 0, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 1, 2);
		terrain_triangle(1, 3, 2);
	} else {
		terrain_triangle(0, 3, 2);
		terrain_triangle(0, 1, 3);
	}

	// Insert vertices

	terrain_position(-1, -1, -1);
	terrain_uvs(F, F);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(-1, -1, +1);
	terrain_uvs(T, F);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(-1, +1, -1);
	terrain_uvs(F, T);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(-1, +1, +1);
	terrain_uvs(T, T);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}

void terrain_east(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(+1, 0, 0);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 2, 1);
		terrain_triangle(2, 3, 1);
	} else {
		terrain_triangle(0, 3, 1);
		terrain_triangle(0, 2, 3);
	}

	// Insert vertices

	terrain_position(+1, -1, -1);
	terrain_uvs(T, F);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(+1, -1, +1);
	terrain_uvs(F, F);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(+1, +1, -1);
	terrain_uvs(T, T);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(+1, +1, +1);
	terrain_uvs(F, T);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}

void terrain_south(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(0, 0, +1);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 2, 1);
		terrain_triangle(2, 3, 1);
	} else {
		terrain_triangle(0, 3, 1);
		terrain_triangle(0, 2, 3);
	}

	// Insert vertices

	terrain_position(-1, -1, +1);
	terrain_uvs(T, F);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(-1, +1, +1);
	terrain_uvs(T, T);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(+1, -1, +1);
	terrain_uvs(F, F);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(+1, +1, +1);
	terrain_uvs(F, T);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}

void terrain_north(float ao[4], float light[4]) {
	// Configure the geometry for the face
	terrain_normals(0, 0, -1);

	// Insert the indices, flipping according to ambient occlusion
	if (ao[0] + ao[3] > ao[1] + ao[2]) {
		terrain_triangle(0, 1, 2);
		terrain_triangle(1, 3, 2);
	} else {
		terrain_triangle(0, 3, 2);
		terrain_triangle(0, 1, 3);
	}

	// Insert vertices

	terrain_position(-1, -1, -1);
	terrain_uvs(F, F);
	terrain_lighting(ao[0], light[0]);
	terrain_vertex();

	terrain_position(-1, +1, -1);
	terrain_uvs(F, T);
	terrain_lighting(ao[1], light[1]);
	terrain_vertex();

	terrain_position(+1, -1, -1);
	terrain_uvs(T, F);
	terrain_lighting(ao[2], light[2]);
	terrain_vertex();

	terrain_position(+1, +1, -1);
	terrain_uvs(T, T);
	terrain_lighting(ao[3], light[3]);
	terrain_vertex();
}
