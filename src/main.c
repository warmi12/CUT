#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "reader.h"
#include "ring_buffer.h"
#include "analyzer.h"

ring_buffer_t* reader_analyzer_ring_buffer;

sem_t reader_analyzer_full_sem;
sem_t reader_analyzer_empty_sem;

int main(){
	printf("hello CUT\n");

        sem_init(&reader_analyzer_full_sem, 0, 0);
        sem_init(&reader_analyzer_empty_sem, 0, 1);

	pthread_t reader_thread;
	pthread_t analyzer_thread;

	pthread_create(&reader_thread,NULL,reader_start,NULL);
	pthread_create(&analyzer_thread,NULL,analyzer_start,NULL);
        pthread_join(reader_thread,NULL);
        pthread_join(analyzer_thread,NULL);

	sem_destroy(&reader_analyzer_full_sem);
	sem_destroy(&reader_analyzer_empty_sem);

	return 0;
}
