#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "threads_info.h"
#include "ring_buffer.h"
#include "analyzer.h"

ring_buffer_t* analyzer_printer_ring_buffer;
static char* reader_analyzer_ring_buffer_item;

extern ring_buffer_t* reader_analyzer_ring_buffer;

extern sem_t full_sem;
extern sem_t print_sem;
extern volatile sig_atomic_t done;

extern uint8_t cpu_number;

static cpu_stats_t* cpu_stats;
static cpu_stats_t* cpu_stats_prev;

static unsigned long long int totald = 0;
static unsigned long long int idled = 0;
static double cpu_usage;

void analyzer_init(void){

	cpu_stats = (cpu_stats_t* ) calloc(cpu_number, sizeof(cpu_stats_t));
	cpu_stats_prev = (cpu_stats_t* ) calloc(cpu_number, sizeof(cpu_stats_t));

	reader_analyzer_ring_buffer_item = (char* )malloc(cpu_number * MAX_NUMBER_OF_VALUES * MAX_STRING_SIZE * sizeof(char));

	analyzer_printer_ring_buffer = ring_buffer_init(sizeof(double));
}

void analyzer_deinit(void){
	
	ring_buffer_deinit(analyzer_printer_ring_buffer);
	free(reader_analyzer_ring_buffer_item);
	free(cpu_stats);
	free(cpu_stats_prev);
}

void* analyzer_loop(void* param){

	UNUSED(param);
	
	while(!done){
       	//      printf("analyzer: wait for semaphore\n");
                sem_wait(&full_sem);
		ring_buffer_get(reader_analyzer_ring_buffer, reader_analyzer_ring_buffer_item);
		analyze_data();
	//	printf("anayzer: data analyzed\n");
                sem_post(&print_sem);
       	}
	pthread_exit(NULL);
}

void analyze_data(void){
	for(uint8_t cpu_counter = 0; cpu_counter < cpu_number; cpu_counter++){
		unsigned long long int* ptr = (unsigned long long int *)&cpu_stats[cpu_counter].user;
		for(uint8_t value_counter = 1; value_counter < MAX_NUMBER_OF_VALUES; value_counter++){
			*ptr = (unsigned long long int)atoll(&reader_analyzer_ring_buffer_item[cpu_counter * MAX_NUMBER_OF_VALUES * MAX_STRING_SIZE + MAX_STRING_SIZE * value_counter]);
			ptr++;
		}
		
       		 	calculate_cpu_usage(&cpu_stats[cpu_counter], &cpu_stats_prev[cpu_counter]);
			ring_buffer_put(analyzer_printer_ring_buffer, &cpu_usage);
			cpu_stats_prev[cpu_counter] = cpu_stats[cpu_counter];
	}
}

void calculate_cpu_usage(cpu_stats_t* cpu_stats_ptr, cpu_stats_t* cpu_stats_prev_ptr){

	cpu_stats_prev_ptr->idle_time = cpu_stats_prev_ptr->idle + cpu_stats_prev_ptr->iowait;
	cpu_stats_ptr->idle_time = cpu_stats_ptr->idle + cpu_stats_ptr->iowait;

	cpu_stats_prev_ptr->non_idle_time = cpu_stats_prev_ptr->user + cpu_stats_prev_ptr->nice + cpu_stats_prev_ptr->system + 
			  cpu_stats_prev_ptr->irq + cpu_stats_prev_ptr->soft_irq + cpu_stats_prev_ptr->steal;

	cpu_stats_ptr->non_idle_time = cpu_stats_ptr->user + cpu_stats_ptr->nice + cpu_stats_ptr->system + 
		     cpu_stats_ptr->irq + cpu_stats_ptr->soft_irq + cpu_stats_ptr->steal;

	cpu_stats_prev_ptr->total_time = cpu_stats_prev_ptr->idle_time + cpu_stats_prev_ptr->non_idle_time;
	cpu_stats_ptr->total_time = cpu_stats_ptr->idle_time + cpu_stats_ptr->non_idle_time;

	totald = cpu_stats_ptr->total_time - cpu_stats_prev_ptr->total_time;
	idled = cpu_stats_ptr->idle_time - cpu_stats_prev_ptr->idle_time;
	
	cpu_usage = (double)(totald - idled) / (double)totald * 100;
}	
