#include <stdio.h>
#include <unistd.h>


#define MAX_LINE 80


int main(void) {


	char *args[MAX_LINE / 2 + 1];

	bool should_run = true;


	while (should_run) {
		printf("osh> ");
		fflush(stdout);


	}

	return 0;
}