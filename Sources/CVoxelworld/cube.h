#pragma once

#include "mesh.h"

void make_cube_wireframe(
    float *data, float x, float y, float z, float n);

void make_character(
    float *data,
    float x, float y, float n, float m, char c);

void make_character_3d(
    float *data, float x, float y, float z, float n, int face, char c);

void make_sphere(float *data, float r, int detail);

//MARK: - Cross Geometry

/// Generates a cross geometry, suitable for turning a 2D image close enough to a 3D cube.
///
/// This method is mainly meant for vegetation.
///
/// - Parameters:
///   - geometry: The geometry to populate, it must have at least 4 faces of headroom.
///   - ao: The ambient occlusion of this block.
///   - light: The light level of this block.
///   - x: The x position of this block.
///   - y: The y position of this block.
///   - z: The z position of this block.
///   - rotation: The Y rotation of the cross in degrees.
void generate_cross_geometry(geometry_t geometry, int id, float ao, float light, float x, float y, float z, float rotation);

//MARK: - Cube Geometry

/// Generates the geometry of a cube.
///
/// Faces are generated in the following order: *left, right, top, bottom, front and back*.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have at least 6 faces of headroom.
///   - id: The texture index per face.
///   - aos: The ambient occlusion per vertex per face.
///   - lights: The light levels per vertex per face.
///   - faces: The faces which should be generated.
///   - x: The x position of the block.
///   - y: The y position of the block.
///   - z: The z position of the block.
void generate_cube_geometry(geometry_t geometry, int id[6], float aos[6][4], float lights[6][4], int faces[6], float x, float y, float z);

/// Generates the bottom geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_bottom_face(geometry_t geometry, float ao[4], float light[4]);

/// Generates the top geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_top_face(geometry_t geometry, float ao[4], float light[4]);

/// Generates the west geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_west_face(geometry_t geometry, float ao[4], float light[4]);

/// Generates the east geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_east_face(geometry_t geometry, float ao[4], float light[4]);

/// Generates the south geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_south_face(geometry_t geometry, float ao[4], float light[4]);

/// Generates the north geometry of a cube.
///
/// ``geometry_position_offset`` and ``geometry_uvs_offset`` must be called first with the block's configuration.
///
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 1 face.
///   - ao: The ambient occlusion of the face's vertices.
///   - light: The light level of the face's vertices.
void generate_north_face(geometry_t geometry, float ao[4], float light[4]);

//MARK: - Player Geometry

/// Generates the geometry of a player
/// - Parameters:
///   - geometry: The geometry to populate, must have space for 6 faces.
///   - x: The x position of the player
///   - y: The y position of the player
///   - z: The z position of the player
///   - rx: The x rotation of the player
///   - ry: The y rotation of the player
void generate_player_geometry(geometry_t geometry, float x, float y, float z, float rx, float ry);
