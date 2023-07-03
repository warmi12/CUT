#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "ring_buffer.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

extern ring_buffer_t* reader_analyzer_ring_buffer;
extern ring_buffer_t* analyzer_printer_ring_buffer;

sem_t full_sem;
sem_t empty_sem;
sem_t print_sem;

static pthread_t reader_thread;
static pthread_t analyzer_thread;
static pthread_t printer_thread;

static volatile sig_atomic_t done = 0;

static void term(){
	done = 1;
}


int main(){

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
    	action.sa_handler = term;
    	sigaction(SIGINT, &action, NULL);


        sem_init(&empty_sem, 0, 1);
	sem_init(&full_sem, 0, 0); 
	sem_init(&print_sem, 0, 0);

	pthread_create(&reader_thread, NULL, reader_start, NULL);
	sleep(1);
	pthread_create(&analyzer_thread, NULL, analyzer_start, NULL);
	sleep(1);
	pthread_create(&printer_thread, NULL, printer_start, NULL);

// 	pthread_join(reader_thread, NULL);
//    	pthread_join(analyzer_thread, NULL);
//	pthread_join(printer_thread, NULL);

	while(!done){
		sleep(1);
	}

	printf("STOP\n");

	ring_buffer_deinit(reader_analyzer_ring_buffer);
	ring_buffer_deinit(analyzer_printer_ring_buffer);

	sem_destroy(&full_sem);
	sem_destroy(&empty_sem);
	sem_destroy(&print_sem);

	return 0;
}
