#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t* buffer){
	buffer->head_idx = 0;
	buffer->tail_idx = 0;
}

bool ring_buffer_is_full(ring_buffer_t* buffer){
	return (((buffer->head_idx - buffer->tail_idx) & RING_BUFFER_MASK) == RING_BUFFER_MASK);
}

bool ring_buffer_is_empty(ring_buffer_t* buffer){
	return (buffer->head_idx == buffer->tail_idx);
}

void ring_buffer_put(ring_buffer_t* buffer, uint8_t *data){
	if(ring_buffer_is_full(buffer)){
		buffer->tail_idx = ((buffer->tail_idx + 1) & RING_BUFFER_MASK);
	}

	buffer->buffer[buffer->head_idx] = *data;
	buffer->head_idx = ((buffer->head_idx + 1) & RING_BUFFER_MASK);
}

bool ring_buffer_get(ring_buffer_t* buffer, uint8_t *data){
	if(ring_buffer_is_empty(buffer)){
		return 0;
	}	
	
	*data = buffer->buffer[buffer->tail_idx];
	buffer->tail_idx = ((buffer -> tail_idx + 1) & RING_BUFFER_MASK);
	return 1;
}
