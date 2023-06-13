#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t* buffer){
	buffer->head_idx = 0;
	buffer->tail_idx = 0;
}


