#ifndef ANALYZER_H
#define ANALYZER_H

#include "ring_buffer.h"

typedef struct{
	unsigned long long int user;
	unsigned long long int nice;
	unsigned long long int system;
	unsigned long long int idle;
	unsigned long long int iowait;
	unsigned long long int irq;
	unsigned long long int soft_irq;
	unsigned long long int steal;
	unsigned long long int guest;
	unsigned long long int guest_nice;

	unsigned long long int idle_time;
	unsigned long long int non_idle_time;
	unsigned long long int total_time;
}cpu_stats_t;

extern ring_buffer_t* analyzer_printer_ring_buffer;

void analyzer_init(void);
void analyzer_deinit(void);
void* analyzer_loop(void* param);
void analyze_data(void);
void calculate_cpu_usage(cpu_stats_t* cpu_stats, cpu_stats_t* cpu_stats_prev);
#endif

