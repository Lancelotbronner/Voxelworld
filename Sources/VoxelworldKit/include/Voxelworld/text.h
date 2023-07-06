//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-07-06.
//

#pragma once

#include <Voxelworld/geometry.h>
#include <Voxelworld/mesh.h>

//MARK: - Text Management

/// Clears the current text geometry.
void text_clear();

/// Uploads the current text geometry to a mesh.
/// - Parameter mesh: The mesh to update with the geometry.
void text_upload(mesh_t mesh);

/// Points all text geometry functions to the given geometry buffer.
/// - Parameter geometry: The geometry buffer to target.
void text_target(geometry_t geometry);

//MARK: - Text Geometry Management

/// Generates the geometry of a string using the current font.
///
/// - Parameters:
///   - characters: The characters of the string.
///   - length: The length of the string.
///   - size: The size of the text.
///   - x: The X offset of the text.
///   - y: The Y offset of the text.
void text_string(char *characters, size_t length, float size, float x, float y);
