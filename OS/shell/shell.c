#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_HISTORY 1000
#define MAX_LINE 80

void readcommand(size_t *args_size, int *need_to_wait, char **args) {

	char buffer_str[MAX_LINE];
	char *buffer = buffer_str;


	while (1) {
		char inputchr = getchar();

		if (inputchr == ' ' || inputchr == '\n') {

			*buffer = 0;
			if (strlen(buffer_str) != 0) {
				args[*args_size] = malloc((1+strlen(buffer_str))*sizeof(char));
				memcpy(args[*args_size], buffer_str, (1+strlen(buffer_str))*sizeof(char));
				++(*args_size);
				buffer = buffer_str;
			}
			if (inputchr == '\n') break;
			continue;
		}
		*buffer = inputchr;
		buffer++;
	}

}

void ChildProcess(int args_size, char **args) {
	char str[MAX_LINE] = "/bin/";
	strcat(str, args[0]);
	execvp(str, args);
	fflush(stdout);
}

int history_len[MAX_HISTORY];
char *history_command[MAX_HISTORY][MAX_LINE/2+1];

int main(void) {

	char *args[MAX_LINE / 2 + 1];
	int should_run = 1;
	int status;
	int history_size = 0;

	int need_to_wait = 1;

	while (should_run) {

		if (need_to_wait) wait(NULL);
		printf("osh> ");
		fflush(stdout);

		size_t args_size = 0;

		readcommand(&args_size, &need_to_wait, args);

		if (args_size == 0) {
			continue;
		}
		if (args_size == 1 && args[0][0] == '!') {
			int which = 0;
			if (args[0][1] == '!') {
				if (history_size == 0) {
					printf("No commands in history.");
					fflush(stdout);
					continue;
				}
				which = history_size-1;
			} else {
				for (int i = 1; i < strlen(args[0]); ++i) {
					which = which * 10 + args[0][i] - '0';
				}
				--which;
			}

			if (which >= history_size) {
				printf("No such command in history.\n");

				fflush(stdout);
				continue;
			}
			for (int i = 0; i < history_len[which]; ++i) {
				printf("%s ", history_command[which][i]);
			}
			puts("");
			fflush(stdout);
			continue;
		}
		if (args_size == 1 && strcmp(args[0], "history") == 0) {
			puts("FUCK");
			for (int i = 0; i < history_size; ++i) {
				printf("%d", i+1);
				for (int j = 0; j < history_len[i]; ++j) {
					printf(" %s", history_command[i][j]);
				}
				puts("");
				fflush(stdout);
			}
			continue;
		}
		history_len[history_size] = args_size;
		for (int i = 0; i < args_size; ++i) {
			history_command[history_size][i] = args[i];
		}
		history_size++;

		if (args_size && strcmp(args[args_size-1], "&") == 0) {
			--args_size;
			need_to_wait = 0;
		}
		args[args_size] = NULL;


		//wait(NULL);
		pid_t pid = fork(); 

		if (pid < 0) {
			fprintf(stderr, "Fork fail");
			return 1;
		}
		else if (pid == 0) { // child process
			ChildProcess(args_size, args);
		}
		
	}


	return 0;
}