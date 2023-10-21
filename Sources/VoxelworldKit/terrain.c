//
//  terrain.c
//  
//
//  Created by Christophe Bronner on 2023-06-29.
//

#include <Voxelworld/terrain.h>
#include <Voxelworld/terrain+vertex.h>

#include <cglm/cglm.h>
#include <OpenGL/gl3.h>
#include <Voxelworld/geometry.h>
#include <Voxelworld/mesh.h>

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

//MARK: - Terrain Management

const char TERRAIN_POSITION_TRANSFORM = 0x1;

struct terrain_s {
	geometry_t geometry;
	struct terrain_vertex_s vertex;
	mat4 position_transform;
	float du, dv;
	float dx, dy, dz;
	unsigned int di;
	bool initialized;
	unsigned char flags;
};

static thread_local struct terrain_s terrain = { };

void terrain_clear() {
	if (terrain.initialized) {
		geometry_clear(terrain.geometry);
		return;
	}
	terrain.geometry = malloc(geometry_sizeof());
	geometry_init(terrain.geometry, 100000, sizeof(struct terrain_vertex_s), 150000, sizeof(unsigned int));
	terrain.initialized = true;
}

void terrain_upload(mesh_t mesh) {
	mesh_update(mesh, terrain.geometry, GL_TRIANGLES);
}

//MARK: - Primitives Management

void terrain_triangle(unsigned int a, unsigned int b, unsigned int c) {
	unsigned int tmp[] = { a, b, c };
	geometry_index(terrain.geometry, tmp, 3 * sizeof(unsigned int));
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

void terrain_position_transform(mat4 transform) {
	glm_mat4_copy(transform, terrain.position_transform);
	terrain.flags |= TERRAIN_POSITION_TRANSFORM;
}

void terrain_position_identity() {
	terrain.flags &= ~TERRAIN_POSITION_TRANSFORM;
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
	if (terrain.flags & TERRAIN_POSITION_TRANSFORM)
		glm_mat4_mulv3(terrain.position_transform, terrain.vertex.position, 0, terrain.vertex.position);
	terrain.vertex.position[0] += terrain.dx;
	terrain.vertex.position[1] += terrain.dy;
	terrain.vertex.position[2] += terrain.dz;
	terrain.vertex.uv[0] += terrain.du;
	terrain.vertex.uv[1] += terrain.dv;
	geometry_vertex(terrain.geometry, &terrain.vertex, sizeof(struct terrain_vertex_s));
}

//MARK: - Geometry Management

static const float S = 1.0 / 16.0;
//static const float F = 1.0 / 2048.0;
//static const float T = S - F;
static const float F = 0; // 1.0 / 2048.0;
static const float T = 1; // S - F;

static inline void calculate_uvs(int w) {
	float du = w % 16 * S;
	float dv = w / 16 * S;
	terrain_uvs_offset(du, dv);
}

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
