//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-07-06.
//

#pragma once

#include <Voxelworld/types.h>

//MARK: - Text Management

/// Clears the current text geometry.
void text_clear();

/// Uploads the current text geometry to a mesh.
///
/// - Parameter mesh: The mesh to update with the geometry.
void text_upload(mesh_t mesh);

//MARK: - Text Geometry Management

/// Generates the geometry of a character using the current font.
///
/// - Parameters:
///  - character: The character.
///  - size: The size of the text.
///  - x: The X offset of the text.
///  - y: The Y offset of the text.
void text_character(char character, float size, float x, float y);

/// Generates the geometry of a string using the current font.
///
/// - Parameters:
///   - characters: The characters of the string.
///   - length: The length of the string.
///   - size: The size of the text.
///   - x: The X offset of the text.
///   - y: The Y offset of the text.
void text_string(char *characters, size_t length, float size, float x, float y);
