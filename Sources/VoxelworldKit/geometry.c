//
//  geometry.c
//  
//
//  Created by Christophe Bronner on 2023-06-22.
//

#include "geometry.h"

#include <stdlib.h>

//MARK: - Lifecycle Management

geometry_t geometry_init(size_t vertices, size_t vertex, size_t indices, size_t index) {
	geometry_t geometry = calloc(1, sizeof(struct geometry_s));
	buffer_init(&geometry->vertices, vertices);
	buffer_init(&geometry->indices, indices);
	geometry->vertex = vertex;
	geometry->index = index;
	geometry->count = 0;
}

void geometry_delete(geometry_t geometry) {
	buffer_delete(&geometry->vertices);
	buffer_delete(&geometry->indices);
	free(geometry);
}

//MARK: - Memory Management

void geometry_capacity(geometry_t geometry, size_t vertex, size_t index) {
	buffer_capacity(&geometry->vertices, vertex);
	buffer_capacity(&geometry->indices, index);
}

void geometry_absolute_capacity(geometry_t geometry, size_t vertex, size_t index) {
	buffer_capacity(&geometry->vertices, vertex);
	buffer_capacity(&geometry->indices, index);
}

void geometry_trim(geometry_t geometry) {
	buffer_trim(&geometry->vertices);
	buffer_trim(&geometry->indices);
}

void geometry_clear(geometry_t geometry) {
	geometry->indices.size = 0;
	geometry->indices.size = 0;
	geometry->count = 0;
}

//MARK: - Vertex Management

void *geometry_vertices(geometry_t geometry, size_t *size) {
	*size = geometry->vertices.size;
	return geometry->vertices.data;
}

size_t geometry_vertex_count(geometry_t geometry) {
	return geometry->vertices.size / geometry->vertex;
}

void geometry_vertex(geometry_t geometry, void *data, size_t size) {
	buffer_push(&geometry->vertices, data, size);
}

//MARK: - Index Management

void *geometry_indices(geometry_t geometry, size_t *size) {
	*size = geometry->indices.size;
	return geometry->indices.data;
}

size_t geometry_index_count(geometry_t geometry) {
	return geometry->indices.size / geometry->index;
}

void geometry_index(geometry_t geometry, void *data, size_t size) {
	buffer_push(&geometry->indices, data, size);
}
