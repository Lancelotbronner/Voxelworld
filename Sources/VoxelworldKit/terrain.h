//
//  terrain.h
//
//
//  Created by Christophe Bronner on 2023-06-29.
//

#pragma once

#include <Voxelworld/terrain+geometry.h>
#include "geometry.h"

//MARK: - Terrain Management

static thread_local struct {
	struct geometry_s geometry;
	struct terrain_vertex_s vertex;
	mat4 position_transform;
	float du, dv;
	float dx, dy, dz;
	unsigned int di;
	bool initialized;
	unsigned char flags;
} terrain;

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
