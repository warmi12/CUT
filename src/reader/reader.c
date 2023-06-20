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

static const char* null_str = "NULL";

extern sem_t reader_analyzer_full_sem;
extern sem_t reader_analyzer_empty_sem;

/*temp solution 
 *null_str tells how long read data in analyzer thread
 */
void init_ring_buffer_item(){
        for(uint8_t i = 0; i < MAX_CPU_NUMBER; i++){
                for(uint8_t j = 0; j < MAX_NUMBER_OF_VALUES; j++){
                        strcpy(&ring_buffer_item[i][j][0], null_str);
                }
        }
}

void* reader_start(void *args){
	size_t ring_buffer_item_size = sizeof(ring_buffer_item);
	reader_analyzer_ring_buffer = ring_buffer_init(ring_buffer_item_size);

	init_ring_buffer_item();

	reader_loop();

	return NULL;
}

void reader_loop(void){
	while(1){
	//	printf("reader: wait for semaphore\n");
		sem_wait(&reader_analyzer_empty_sem);
		file = fopen("/proc/stat","r");
		if(file){
			reader_parse_file();
	//		printf("reader: added data to buffer\n");
			fclose(file);
			sem_post(&reader_analyzer_full_sem);
			sleep(1);
		}
		else{
			//info to logger
			//error
		}
	}
	
	//remember about it
	//ring_buffer_deinit(reader_analyzer_ring_buffer);
}

void reader_parse_file(void){
	uint8_t cpu_number = 0;
	uint8_t value_number = 0;

	fgets(line_from_file, MAX_LINE_SIZE, file);
	while(strstr(line_from_file, STOP_DELIMITER) == NULL){ 			//read line until "itr" str 
		parsed_data = strtok(line_from_file, VALUES_DELIMITER);		//parse space separated data
		while(parsed_data != NULL){
			strcpy(&ring_buffer_item[cpu_number][value_number][0], parsed_data); //put parsed data to temp buffer
			parsed_data = strtok(NULL, " ");

			value_number++;
		}
		value_number=0;
		cpu_number++;
		fgets(line_from_file, MAX_LINE_SIZE, file);	
	}

	ring_buffer_put(reader_analyzer_ring_buffer, &ring_buffer_item); 	//put data to ring buffer
}
