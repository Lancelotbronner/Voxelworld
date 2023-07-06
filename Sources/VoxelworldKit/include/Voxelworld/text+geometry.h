//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-07-06.
//

#pragma once

#include <cglm/types.h>

//MARK: - Vertex Management

struct text_vertex_s {
	float position[3];
	float uv[2];
};

/// Configures the OpenGL vertex attributes of `text_vertex_s`
void vertex_text();

//MARK: - Primitives Management

/// Assigns the calculated UVs for the specified character.
///
/// - Parameter character: The character to use as index for UVs.
void text_character(char character);

/// Assigns the given UVs to the current character.
///
/// - Parameters:
///   - u: The value of the `u` component.
///   - v: The value of the `v` component.
void text_uvs(float u, float v);

/// Applies the given size to futur characters.
///
/// - Parameter size: The width of characters.
void text_size(float size);

/// Applies the given position offset to future characters.
///
/// - Parameters:
///   - dx: The offset in the `x` component.
///   - dy: The offset in the `y` component.
///   - dz: The offset in the `z` component.
void text_position_offset(float dx, float dy, float dz);

/// Assigns the given position to the current vertex, relative to the beginning of the text.
///
/// - Parameters:
///   - x: The value of the `x` component.
///   - y: The value of the `y` component.
void text_position(float x, float y);

/// Commits the current vertex to the current geometry.
void text_vertex();
