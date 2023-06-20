#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <float.h>
#include <string.h>
#include "ring_buffer.h"
#include "reader.h"
#include "analyzer.h"

extern ring_buffer_t* reader_analyzer_ring_buffer;
static char ring_buffer_item[MAX_CPU_NUMBER][MAX_NUMBER_OF_VALUES][MAX_SINGLE_STRING_SIZE];

extern sem_t reader_analyzer_full_sem;
extern sem_t reader_analyzer_empty_sem;

static cpu_stats_t cpu_stats[MAX_CPU_NUMBER] = {0};
static cpu_stats_t cpu_stats_prev[MAX_CPU_NUMBER] = {0};

static char* null_str = "NULL";

static unsigned long long int totald = 0;
static unsigned long long int idled = 0;
static double cpu_usage;

void* analyzer_start(void *args){
	analyzer_loop();
	return NULL;
}

void analyzer_loop(void){
        while(1){
       	//      printf("analyzer: wait for semaphore\n");
                sem_wait(&reader_analyzer_full_sem);
		ring_buffer_get(reader_analyzer_ring_buffer, &ring_buffer_item);
		analyze_data();
	//	printf("anayzer: data analyzed\n");
                sem_post(&reader_analyzer_empty_sem);
       }
}

void analyze_data(void){
	uint8_t cpu_number = 0;

	while(strcmp(&ring_buffer_item[cpu_number][0][0], null_str)){
		unsigned long long int* ptr = (unsigned long long int *)&cpu_stats[cpu_number].user;
		for(uint8_t value_number = 1; value_number < MAX_NUMBER_OF_VALUES; value_number++){
			*ptr = (unsigned long long int)atoll(&ring_buffer_item[cpu_number][value_number][0]);
			ptr++;
		}
		
        	calculate_cpu_usage(&cpu_stats[cpu_number], &cpu_stats_prev[cpu_number]);
		cpu_stats_prev[cpu_number] = cpu_stats[cpu_number];
		cpu_number++;
	}
	printf("##############\n");
}

double calculate_cpu_usage(cpu_stats_t* cpu_stats_ptr, cpu_stats_t* cpu_stats_prev_ptr){

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
	printf("%f%\n",cpu_usage);
	return cpu_usage;
}	
