#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define RING_BUFFER_SIZE 256 			//RING_BUFFER_SIZE must be power of two
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)

typedef struct{
	uint8_t buffer[RING_BUFFER_SIZE];
	uint8_t head_idx;
	uint8_t tail_idx;
}ring_buffer_t;


void ring_buffer_init(ring_buffer_t* buffer);

bool ring_buffer_is_full(ring_buffer_t* buffer);

bool ring_buffer_is_empty(ring_buffer_t* buffer);

void ring_buffer_put(ring_buffer_t* buffer, uint8_t* data);

bool ring_buffer_get(ring_buffer_t* buffer, uint8_t* data);

#endif
