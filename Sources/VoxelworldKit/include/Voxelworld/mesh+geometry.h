//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-06-27.
//

#pragma once

#include <Voxelworld/geometry.h>
#include <Voxelworld/mesh.h>

//MARK: - Geometry Integration

/// Updates an existing mesh with geometry.
/// - Parameters:
///   - mesh: The mesh to populate.
///   - geometry: The geometry to upload.
///   - primitive: The primitive this geometry represents.
void mesh_update(mesh_t mesh, geometry_t geometry, int32_t primitive);
