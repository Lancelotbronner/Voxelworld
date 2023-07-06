//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-06-29.
//

#pragma once

#include <Voxelworld/terrain.h>

#include <cglm/types.h>

//MARK: - Vertex Management

struct terrain_vertex_s {
	float position[3];
	float normal[3];
	float uv[2];
	float ao;
	float light;
};

/// Configures the OpenGL vertex attributes of `terrain_vertex_s`
void vertex_terrain();

//MARK: - Primitives Management

/// Inserts a triangle into the terrain geometry.
///
/// - Parameters:
///   - a: The first index of the triangle.
///   - b: The second index of the triangle.
///   - c: The third index of the triangle.
void terrain_triangle(unsigned int a, unsigned int b, unsigned int c);

/// Applies the given UV offset to futur vertices.
///
/// - Parameters:
///   - du: The offset in the `u` component.
///   - dv: The offset in the `v` component.
void terrain_uvs_offset(float du, float dv);

/// Applies the given position offset to future vertices.
///
/// - Parameters:
///   - dx: The offset in the `x` component.
///   - dy: The offset in the `y` component.
///   - dz: The offset in the `z` component.
void terrain_position_offset(float dx, float dy, float dz);

/// Enables the given transform to futur vertex positions
///
///  - Parameter transform: The transformation matrix to apply to futur vertices.
void terrain_position_transform(mat4 transform);

/// Disables the current position transformation
void terrain_position_identity();

/// Assigns the given position to the current vertex, relative to the chunk.
///
/// - Parameters:
///   - x: The value of the `x` component.
///   - y: The value of the `y` component.
///   - z: The value of the `z` component.
void terrain_position(float x, float y, float z);

/// Assigns the given normals to the current vertex.
///
/// - Parameters:
///   - x: The value of the `x` component.
///   - y: The value of the `y` component.
///   - z: The value of the `z` component.
void terrain_normals(float x, float y, float z);

/// Assigns the given UVs to the current vertex.
///
/// - Parameters:
///   - u: The value of the `u` component.
///   - v: The value of the `v` component.
void terrain_uvs(float u, float v);

/// Assigns the given AO and light level to the current vertex.
///
/// - Parameters:
///   - ao: The value of the ambient occlusion for this vertex.
///   - light: The light level at this vertex.
void terrain_lighting(float ao, float light);

/// Commits the current vertex to the terrain's geometry.
void terrain_vertex();

//MARK: - Geometry Management

/// Generates the bottom geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_bottom(float ao[4], float light[4]);

/// Generates the top geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_top(float ao[4], float light[4]);

/// Generates the west geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_west(float ao[4], float light[4]);

/// Generates the east geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_east(float ao[4], float light[4]);

/// Generates the south geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_south(float ao[4], float light[4]);

/// Generates the north geometry of a cube.
///
/// ``terrain_position_offset`` and ``terrain_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void terrain_north(float ao[4], float light[4]);
