#include <string.h>
#include <stdlib.h>
#include "threads_info.h"
#include "ring_buffer.h"
#include "reader.h"

ring_buffer_t* reader_analyzer_ring_buffer;
static char* reader_analyzer_ring_buffer_item;

static FILE* file;
static char line_from_file[MAX_LINE_SIZE];
static char* parsed_data;

extern sem_t empty_sem;
extern sem_t full_sem;
extern volatile sig_atomic_t done;

extern uint8_t cpu_number;

uint8_t get_cpu_number(void){
	
	uint8_t cpu_counter = 0;
	
	file = fopen("/proc/stat","r");
	fgets(line_from_file, MAX_LINE_SIZE, file);
       	while(strstr(line_from_file, STOP_DELIMITER) == NULL){
	      	cpu_counter++;
		fgets(line_from_file, MAX_LINE_SIZE, file);
	}
	fclose(file);

	return cpu_counter;
}

void reader_init(void){

	size_t reader_analyzer_ring_buffer_item_size = (size_t)cpu_number * MAX_NUMBER_OF_VALUES * MAX_STRING_SIZE;
	reader_analyzer_ring_buffer_item = (char* )malloc(cpu_number * MAX_NUMBER_OF_VALUES * MAX_STRING_SIZE * sizeof(char));
		
        reader_analyzer_ring_buffer = ring_buffer_init(reader_analyzer_ring_buffer_item_size);
}

void reader_deinit(void){

	ring_buffer_deinit(reader_analyzer_ring_buffer);
	free(reader_analyzer_ring_buffer_item);
}

void* reader_loop(void* param){

	UNUSED(param);

	while(!done){
	//	printf("reader: wait for semaphore\n");
		sem_wait(&empty_sem);
		file = fopen("/proc/stat","r");
		if(file){
			reader_parse_file();
			fclose(file);
			sem_post(&full_sem);
		}
		else{
			//info to logger
			//error
		}
	}
	pthread_exit(NULL);
}

void reader_parse_file(void){

	uint8_t value_counter = 0;

	fgets(line_from_file, MAX_LINE_SIZE, file);
	for(uint8_t cpu_counter = 0; cpu_counter < cpu_number; cpu_counter++){ 			//read line until "itr" str 
		parsed_data = strtok(line_from_file, VALUES_DELIMITER);				//parse space separated data
	
		while(parsed_data != NULL){
			strcpy(&reader_analyzer_ring_buffer_item[cpu_counter * MAX_NUMBER_OF_VALUES * MAX_STRING_SIZE + value_counter * MAX_STRING_SIZE], parsed_data);
			parsed_data = strtok(NULL, " ");
			value_counter++;
		}
	
		value_counter = 0;

		fgets(line_from_file, MAX_LINE_SIZE, file);	
	}
	
	ring_buffer_put(reader_analyzer_ring_buffer, reader_analyzer_ring_buffer_item); 			//put data to ring buffer
}
