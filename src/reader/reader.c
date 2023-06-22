#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include "reader.h"
#include "ring_buffer.h"

extern ring_buffer_t* reader_analyzer_ring_buffer;

static FILE* file;
static char line_from_file[MAX_LINE_SIZE];
static char ring_buffer_item[MAX_CPU_NUMBER][MAX_NUMBER_OF_VALUES][MAX_SINGLE_STRING_SIZE];
static char* parsed_data;

extern sem_t empty_sem;
extern sem_t full_sem;

uint8_t cpu_number;

void get_cpu_number(void){
	file = fopen("/proc/stat","r");
	fgets(line_from_file, MAX_LINE_SIZE, file);
       	while(strstr(line_from_file, STOP_DELIMITER) == NULL){
	      	cpu_number++;
		fgets(line_from_file, MAX_LINE_SIZE, file);
	}
	fclose(file);
}

void* reader_start(void* param){
	size_t ring_buffer_item_size = sizeof(ring_buffer_item);
	reader_analyzer_ring_buffer = ring_buffer_init(ring_buffer_item_size);

	get_cpu_number();

	reader_loop();

	return NULL;
}

void reader_loop(void){
	while(1){
	//	printf("reader: wait for semaphore\n");
		sem_wait(&empty_sem);
		file = fopen("/proc/stat","r");
		if(file){
			reader_parse_file();
	//		printf("reader: added data to buffer\n");
			fclose(file);
			sem_post(&full_sem);
		}
		else{
			//info to logger
			//error
		}
	}
}

void reader_parse_file(void){

	fgets(line_from_file, MAX_LINE_SIZE, file);
	for(uint8_t cpu_counter = 0; cpu_counter < cpu_number; cpu_counter++){ 			//read line until "itr" str 
		parsed_data = strtok(line_from_file, VALUES_DELIMITER);				//parse space separated data
	
		uint8_t value_counter = 0;
		while(parsed_data != NULL){
			strcpy(&ring_buffer_item[cpu_counter][value_counter][0], parsed_data); //put parsed data to temp buffer
			parsed_data = strtok(NULL, " ");

			value_counter++;
		}
		fgets(line_from_file, MAX_LINE_SIZE, file);	
	}

	ring_buffer_put(reader_analyzer_ring_buffer, &ring_buffer_item); 			//put data to ring buffer
}
