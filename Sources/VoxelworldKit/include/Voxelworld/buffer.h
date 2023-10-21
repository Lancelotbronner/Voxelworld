//
//  buffer.h
//  
//
//  Created by Christophe Bronner on 2023-06-28.
//

#pragma once

#include <Voxelworld/types.h>

//MARK: - Structures

struct buffer_s {
	void *data;
	size_t size;
	size_t capacity;
};

//MARK: - Lifecycle Management

void buffer_init(buffer_t buffer, size_t capacity);

void buffer_delete(buffer_t buffer);

//MARK: - Memory Management

void buffer_capacity(buffer_t buffer, size_t capacity);

void buffer_absolute_capacity(buffer_t buffer, size_t capacity);

void buffer_trim(buffer_t buffer);

//MARK: - Content Management

void buffer_write(buffer_t buffer, size_t offset, void *data, size_t size);

void buffer_push(buffer_t buffer, void *data, size_t size);
