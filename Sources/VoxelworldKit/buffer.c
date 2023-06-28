//
//  buffer.c
//  
//
//  Created by Christophe Bronner on 2023-06-28.
//

#include "buffer.h"

#include <stdlib.h>
#include <string.h>

//MARK: - Lifecycle Management

void buffer_init(buffer_t buffer, size_t capacity) {
	buffer->data = malloc(capacity);
	buffer->capacity = capacity;
	buffer->size = 0;
}

void buffer_delete(buffer_t buffer) {
	free(buffer->data);
	buffer->capacity = 0;
	buffer->size = 0;
}

//MARK: - Memory Management

void buffer_capacity(buffer_t buffer, size_t capacity) {
	buffer_absolute_capacity(buffer, buffer->size + capacity);
}

void buffer_absolute_capacity(buffer_t buffer, size_t capacity) {
	if (buffer->capacity < capacity) {
		do buffer->capacity *= 2;
		while (buffer->capacity < capacity);
		buffer->data = realloc(buffer->data, buffer->capacity);
	}
}

void buffer_trim(buffer_t buffer) {
	buffer->data = realloc(buffer->data, buffer->size);
	buffer->capacity = buffer->size;
}

//MARK: - Content Management

void buffer_write(buffer_t buffer, size_t offset, void *data, size_t size) {
	memcpy(buffer->data + offset, data, size);
}

void buffer_push(buffer_t buffer, void *data, size_t size) {
	buffer_write(buffer, buffer->size, data, size);
}
