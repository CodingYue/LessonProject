#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


#define MAX_LINE 80

void readcommand(size_t *args_size, int *need_to_wait, char **args) {

	char buffer_str[MAX_LINE];
	char *buffer = buffer_str;


	while (1) {
		char inputchr = getchar();

		if (inputchr == ' ' || inputchr == '\n') {

			*buffer = 0;
			if (strlen(buffer_str) != 0) {
				args[*args_size] = malloc(strlen(buffer_str)*sizeof(char));
				memcpy(args[*args_size], buffer_str, strlen(buffer_str)*sizeof(char));
				++(*args_size);
				buffer = buffer_str;
			}
			if (inputchr == '\n') break;
			continue;
		}
		*buffer = inputchr;
		buffer++;
	}


	if (*args_size && strcmp(args[*args_size-1], "&") == 0) {
		--*args_size;
		*need_to_wait = 0;
	}
	args[*args_size] = NULL;
}

void ChildProcess(int args_size, char **args) {
	char str[MAX_LINE] = "/bin/";
	strcat(str, args[0]);
	execvp(str, args);
}

int main(void) {

	char *args[MAX_LINE / 2 + 1];
	int should_run = 1;
	int status;


	while (should_run) {
		printf("osh> ");
		fflush(stdout);

		size_t args_size = 0;
		int need_to_wait = 1;

		readcommand(&args_size, &need_to_wait, args);

		if (args_size == 0) {
			continue;
		}

		printf("need_to_wait = %d\n", need_to_wait);
		fflush(stdout);


		pid_t pid = fork(); 

		if (pid < 0) {
			fprintf(stderr, "Fork fail");
			return 1;
		}
		else if (pid == 0) { // child process
			ChildProcess(args_size, args);
			exit(0);
		}
		if (need_to_wait) wait(&status);
		
		
	}


	return 0;
}