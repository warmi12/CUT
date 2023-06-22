#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "ring_buffer.h"
#include "printer.h"

extern sem_t print_sem;
extern sem_t empty_sem;
extern ring_buffer_t* analyzer_printer_ring_buffer;

extern uint8_t cpu_number;
static double cpu_usage = 0;

void * printer_start(void *param){
	
	printer_loop();
	return NULL;
}

void printer_loop(void){
	while(1){
		//printf("printer: wait for semaphore\n");
		sem_wait(&print_sem);	
		system("clear");
		for(uint8_t cpu_counter = 0; cpu_counter<cpu_number; cpu_counter++){
			ring_buffer_get(analyzer_printer_ring_buffer, &cpu_usage);
			if(cpu_counter == 0){
				printf("Total: %f %%\n", cpu_usage);
			}
			else{
				printf("cpu%d %f %%\n", cpu_counter - 1, cpu_usage);
			}
		}
		sleep(1);
		sem_post(&empty_sem);

	}	
}



