#ifndef READER_H
#define READER_H

#include "ring_buffer.h"

extern ring_buffer_t* reader_analyzer_ring_buffer;

void reader_init(void);
void* reader_loop(void* param);
void reader_parse_file(void);
void reader_deinit(void);

uint8_t get_cpu_number(void);
#endif
