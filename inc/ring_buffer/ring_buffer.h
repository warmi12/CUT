#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define RING_BUFFER_SIZE 256 			//RING_BUFFER_SIZE must be power of two
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)

typedef struct{
	char* buffer;
	size_t item_size;
	int head_idx;
	int tail_idx;
}ring_buffer_t;


ring_buffer_t* ring_buffer_init(size_t item_size);

void ring_buffer_deinit(ring_buffer_t* ring_buffer);

bool ring_buffer_is_full(ring_buffer_t* ring_buffer);

bool ring_buffer_is_empty(ring_buffer_t* ring_buffer);

void ring_buffer_put(ring_buffer_t* ring_buffer, void* item);

bool ring_buffer_get(ring_buffer_t* ring_buffer, void* item);

#endif
