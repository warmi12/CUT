#ifndef THREAD_INFO_H
#define THREAD_INFO_H

#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#define MAX_STRING_SIZE 20
#define MAX_NUMBER_OF_VALUES 11 
#define MAX_LINE_SIZE MAX_STRING_SIZE*MAX_NUMBER_OF_VALUES
#define STOP_DELIMITER "intr"
#define VALUES_DELIMITER " "
#define UNUSED(x) (void)(x)

extern sem_t full_sem;
extern sem_t empty_sem;
extern sem_t print_sem;

extern pthread_t reader_thread;
extern pthread_t analyzer_thread;
extern pthread_t printer_thread;

extern uint8_t cpu_number; 

extern volatile sig_atomic_t done;

void threads_init(void);
void threads_create(void);
void threads_run(void);
void threads_term(int signum);
void threads_deinit(void);

#endif
