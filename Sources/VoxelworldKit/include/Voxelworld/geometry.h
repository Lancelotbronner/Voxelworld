//
//  Geometry.h
//
//
//  Created by Christophe Bronner on 2023-06-22.
//

#pragma once

#include <stdint.h>

//MARK: - Structures

typedef struct geometry_s *geometry_t;

//MARK: - Lifecycle Management

/// Initializes an empty geometry with the specified capacity.
/// - Parameters:
///  - vertices: The capacity for vertices.
///  - vertex: The size of a single vertex, in bytes.
///  - indices: The capacity for indices.
///  - index: The size of a single index, in bytes.
geometry_t geometry_init(size_t vertices, size_t vertex, size_t indices, size_t index);

/// Deallocates the geometry
/// - Parameter geometry: The geometry to deallocate.
void geometry_delete(geometry_t geometry);

//MARK: - Memory Management

/// Ensures the geometry has enough capacity leftover.
/// - Parameters:
///   - geometry: The geometry to check.
///   - vertex: The number of bytes required in the vertex buffer.
///   - index: The number of bytes required in the index buffer.
void geometry_capacity(geometry_t geometry, size_t vertex, size_t index);

/// Ensures the geometry has enough capacity.
/// - Parameters:
///   - geometry: The geometry to check.
///   - vertex: The minimum number of bytes in the vertex buffer.
///   - index: The minimum number of bytes in the index buffer.
void geometry_absolute_capacity(geometry_t geometry, size_t vertex, size_t index);

/// Trims the geometry to use as little memory as possible.
/// - Parameter geometry: The geometry to trim.
void geometry_trim(geometry_t geometry);

/// Clears data from the geometry in order to reuse it.
/// - Parameter geometry: The geometry to clear.
void geometry_clear(geometry_t geometry);

//MARK: - Vertex Management

/// Retrieves the vertex buffer of a geometry.
/// - Parameters:
///   - geometry: The geometry from which to retrieve the vertex buffer.
///   - size: The size of the buffer in bytes.
///  - Returns: The vertex buffer.
void *geometry_vertices(geometry_t geometry, size_t *size);

/// Returns the number of vertices in a geometry.
/// - Parameter geometry: The geometry from which to take the information.
size_t geometry_vertex_count(geometry_t geometry);

/// Inserts a vertex into the geometry.
/// - Parameters:
///   - geometry: The geometry in which to insert the vertex.
///   - data: The data to insert into the geometry.
///   - count: The number of vertices to insert.
void geometry_vertex(geometry_t geometry, void *data, size_t count);

//MARK: - Index Management

/// Retrieves the index buffer of a geometry.
/// - Parameters:
///   - geometry: The geometry from which to retrieve the index buffer.
///   - size: The size of the buffer in bytes.
///  - Returns: The index buffer.
void *geometry_indices(geometry_t geometry, size_t *size);

/// Returns the number of indices in a geometry.
/// - Parameter geometry: The geometry from which to take the information.
size_t geometry_index_count(geometry_t geometry);

/// Inserts a triangle into the geometry. Increments the primitive count.
/// - Parameters:
///   - geometry: The geometry in which to insert the triangle.
///   - data: The data to insert into the geometry.
///   - count: The number of indices to insert.
void geometry_index(geometry_t geometry, void *data, size_t count);
