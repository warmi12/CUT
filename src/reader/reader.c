#include <string.h>
#include <stdio.h>
#include "reader.h"

//plan dzialania:
//otwarcie pliku
//odczyt danych 
//przypisanie do struktury
//umieszczenie w ring_buffer
//
//init proces? czyli otwarcie pliku? sprawdzenie czy wszystko jest ok?
//z maina wywolanie funkcji run_reader() lub reader_start()
//calosc w petli ma dzialac
//u64 sa te liczby w proc/stat -> max_bytes per line = 214

FILE* fptr;
char line[MAX_LINE_SIZE];
char* parsed_data;

void reader_start(){
	reader_loop();
}

void reader_parse_file(){
	fgets(line, MAX_LINE_SIZE, fptr);
	while(strstr(line,"intr") == NULL){	
		parsed_data = strtok(line, " ");
		while(parsed_data != NULL){
			printf("%s ", parsed_data);
			parsed_data = strtok(NULL, " ");
			//add to ring_buffer
		}
		fgets(line,MAX_LINE_SIZE,fptr);
	}
}

void reader_loop(){
	printf("reader\n");
	//while(1){
		//czekamy na semafor
		fptr = fopen("/proc/stat","r");
		if(fptr != NULL){
			reader_parse_file();
			fclose(fptr);
		}
		else{
			//koniec programu
			//wyslij do loggera w przyszlosci
		}
		

	//}
}
