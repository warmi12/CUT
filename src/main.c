#include <stdio.h>
#include <unistd.h>
#include "threads_info.h"

int main(){

	threads_init();
	threads_create();
	threads_run();
	threads_deinit();

	printf("end\n");
	return 0;
}
