#include "string.h"
#include "threads_info.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

sem_t full_sem;
sem_t empty_sem;
sem_t print_sem;

pthread_t reader_thread;
pthread_t analyzer_thread;
pthread_t printer_thread;

uint8_t cpu_number = 0;

volatile sig_atomic_t done = 0;

void threads_init(void){

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
    	action.sa_handler = threads_term;
    	sigaction(SIGINT, &action, NULL);

	sem_init(&empty_sem, 0, 1);
	sem_init(&full_sem, 0, 0);
	sem_init(&print_sem, 0, 0);

	cpu_number = get_cpu_number();
	reader_init();
	analyzer_init();
}
	
void threads_create(void){

	pthread_create(&reader_thread, NULL, reader_loop, NULL);
	pthread_create(&analyzer_thread, NULL, analyzer_loop, NULL);
	pthread_create(&printer_thread, NULL, printer_loop, NULL);
}
void threads_run(void){

	pthread_join(reader_thread, NULL);
    	pthread_join(analyzer_thread, NULL);
	pthread_join(printer_thread, NULL);
}
void threads_term(int signum){

	UNUSED(signum);
	done=1;
}
void threads_deinit(void){
	
	sem_destroy(&full_sem);
	sem_destroy(&empty_sem);
	sem_destroy(&print_sem);

	reader_deinit();
	analyzer_deinit();
}

