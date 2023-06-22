#ifndef READER_H
#define READER_H

#define MAX_SINGLE_STRING_SIZE 20
#define MAX_NUMBER_OF_VALUES 11 
#define MAX_LINE_SIZE MAX_SINGLE_STRING_SIZE*MAX_NUMBER_OF_VALUES
#define MAX_CPU_NUMBER 10
#define STOP_DELIMITER "intr"
#define VALUES_DELIMITER " "
#define READER_ANALYZER_DELIMITER "NULL"

void* reader_start(void* param);
void reader_loop(void);
void reader_parse_file(void);

void get_cpu_number(void);
#endif
