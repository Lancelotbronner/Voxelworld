//
//  Header.h
//  
//
//  Created by Christophe Bronner on 2023-07-06.
//

#pragma once

//MARK: - Vertex Management

struct text_vertex_s {
	float position[2];
	float uv[2];
};

/// Configures the OpenGL vertex attributes of `text_vertex_s`
void vertex_text();

//MARK: - Primitives Management

/// Inserts a triangle into the text geometry.
///
/// - Parameters:
///   - a: The first index of the triangle.
///   - b: The second index of the triangle.
///   - c: The third index of the triangle.
void text_triangle(unsigned int a, unsigned int b, unsigned int c);

/// Applies the given UV offset to futur vertices.
///
/// - Parameters:
///   - du: The offset in the `u` component.
///   - dv: The offset in the `v` component.
void text_uvs_offset(float du, float dv);

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
void text_position_offset(float dx, float dy);

/// Assigns the given position to the current vertex, relative to the beginning of the text.
///
/// - Parameters:
///   - x: The value of the `x` component.
///   - y: The value of the `y` component.
void text_position(float x, float y);

/// Commits the current vertex to the current geometry.
void text_vertex();
