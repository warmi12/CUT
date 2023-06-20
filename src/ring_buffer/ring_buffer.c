#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"

ring_buffer_t* ring_buffer_init(size_t item_size){
	ring_buffer_t* ring_buffer = (ring_buffer_t* )malloc(sizeof(ring_buffer));
	ring_buffer->buffer = (char* )malloc(RING_BUFFER_SIZE * item_size);
	ring_buffer->item_size = item_size;
	ring_buffer->head_idx = 0;
	ring_buffer->tail_idx = 0;

	return ring_buffer;
}

void ring_buffer_deinit(ring_buffer_t* ring_buffer){
	free(ring_buffer->buffer);
	free(ring_buffer);
}

bool ring_buffer_is_full(ring_buffer_t* ring_buffer){
	return (((ring_buffer->head_idx - ring_buffer->tail_idx) & RING_BUFFER_MASK) == RING_BUFFER_MASK);
}

bool ring_buffer_is_empty(ring_buffer_t* ring_buffer){
	return (ring_buffer->head_idx == ring_buffer->tail_idx);
}

void ring_buffer_put(ring_buffer_t* ring_buffer, void *item){
	if(ring_buffer_is_full(ring_buffer)){
		ring_buffer->tail_idx = ((ring_buffer->tail_idx + 1) & RING_BUFFER_MASK);
	}

	void* destination_addr = (char* )ring_buffer->buffer + (ring_buffer->head_idx * ring_buffer->item_size);
	memcpy(destination_addr, item, ring_buffer->item_size);
	ring_buffer->head_idx = ((ring_buffer->head_idx + 1) & RING_BUFFER_MASK);
}

bool ring_buffer_get(ring_buffer_t* ring_buffer, void* item){
	if(ring_buffer_is_empty(ring_buffer)){
		return 0;
	}	
	
	void* source_addr = (char* )ring_buffer->buffer + (ring_buffer->tail_idx * ring_buffer->item_size);
	memcpy(item, source_addr, ring_buffer->item_size);
	ring_buffer->tail_idx = ((ring_buffer -> tail_idx + 1) & RING_BUFFER_MASK);
	return 1;
}
