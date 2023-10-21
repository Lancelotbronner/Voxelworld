//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-06-29.
//

#include <Voxelworld/types.h>

//MARK: - Terrain Management

/// Clears the current terrain geometry.
void terrain_clear();

/// Uploads the current terrain to a mesh.
/// - Parameter mesh: The mesh to update with the geometry.
void terrain_upload(mesh_t mesh);

//MARK: - Terrain Geometry Management

/// Inserts a cross geometry into the terrain, suitable for turning a 2D image close enough to a 3D cube.
///
/// This method is mainly meant for vegetation.
///
/// - Parameters:
///   - texture: The texture index of this block.
///   - ao: The ambient occlusion of this block.
///   - light: The light level of this block.
///   - x: The x position of this block.
///   - y: The y position of this block.
///   - z: The z position of this block.
///   - rotation: The Y rotation of the cross in degrees.
void terrain_cross(int texture, float ao, float light, float x, float y, float z, float rotation);

/// Generates the geometry of a cube.
///
/// Faces are generated in the following order: *left, right, top, bottom, front and back*.
///
/// - Parameters:
///   - textures: The texture index per face.
///   - aos: The ambient occlusion per vertex per face.
///   - lights: The light levels per vertex per face.
///   - faces: The faces which should be generated.
///   - x: The x position of the block.
///   - y: The y position of the block.
///   - z: The z position of the block.
void terrain_cube(int textures[6], float aos[6][4], float lights[6][4], unsigned char faces, float x, float y, float z);

