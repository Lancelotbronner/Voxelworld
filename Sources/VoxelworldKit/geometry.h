//
//  geometry.h
//  
//
//  Created by Christophe Bronner on 2023-06-22.
//

#pragma once

#include <Voxelworld/geometry.h>

#include <Voxelworld/buffer.h>

struct geometry_s {
	struct buffer_s indices;
	struct buffer_s vertices;
	size_t count;
	uint16_t vertex;
	uint8_t index;
};
