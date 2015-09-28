#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX_HISTORY 1000
#define MAX_LINE 80

void proc_exit() {
	int status;
	pid_t pid;
	while (1) {
		pid = wait(NULL);
		if (pid == 0) return;
		if (pid == -1) return;
	}
}

int readcommand(char **args) {

	int size = 0;

	char buffer_str[MAX_LINE];
	char *buffer = buffer_str;


	while (1) {
		char inputchr = getchar();

		if (inputchr == ' ' || inputchr == '\n') {

			*buffer = 0;
			if (strlen(buffer_str) != 0) {
				args[size] = malloc((1+strlen(buffer_str))*sizeof(char));
				memcpy(args[size], buffer_str, (1+strlen(buffer_str))*sizeof(char));
				++size;
				buffer = buffer_str;
			}
			if (inputchr == '\n') break;
			continue;
		}
		*buffer = inputchr;
		buffer++;
	}
	return size;
}

void ChildProcess(int args_size, char **args) {
	char str[MAX_LINE] = "/bin/";
	strcat(str, args[0]);
	execvp(str, args);
}

int history_len[MAX_HISTORY];
char *history_command[MAX_HISTORY][MAX_LINE/2+1];

int main(void) {

	signal (SIGCHLD, proc_exit);

	char *args[MAX_LINE / 2 + 1];
	int should_run = 1;
	int status;
	int history_size = 0;

	while (should_run) {

		int need_to_wait = 1;

		printf("osh> ");
		fflush(stdout);

		size_t args_size = 0;

		args_size = readcommand(args);


		if (args_size == 0) {
			continue;
		}
		if (args_size == 1 && strcmp(args[0], "exit") == 0) {
			break;
		}
		if (strcmp(args[0], "cd") == 0) {
			int result = chdir(args[1]);
			if (result != 0) {
				switch (errno) {
					case ENOENT:
						puts("The file does not exist.");
						break;
					case ENOTDIR:
						puts("A component of path is not a directory.");
						break;
					case EACCES:
						puts("Search permission is denied for one of the components of path.");
						break;
				}
				fflush(stdout);
			}
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
				sscanf(args[0]+1, "%d", &which);
				--which;
			}

			if (which >= history_size) {
				printf("No such command in history.\n");
				fflush(stdout);
				continue;
			}		
			args_size = history_len[which];
			for (int i = 0; i < args_size; ++i) args[i] = history_command[which][i];
		}


		if (args_size == 1 && strcmp(args[0], "history") == 0) {
			
			for (int i = 0; i < history_size; ++i) {
				printf("%d", i+1);
				for (int j = 0; j < history_len[i]; ++j) {
					printf(" %s", history_command[i][j]);
				}
				puts("");
				fflush(stdout);
			}

		}

		history_len[history_size] = args_size;
		for (int i = 0; i < args_size; ++i) {
			history_command[history_size][i] = args[i];
		}
		history_size++;

		if (args_size == 1 && strcmp(args[0], "history") == 0) continue;

		if (args_size && strcmp(args[args_size-1], "&") == 0) {
			--args_size;
			need_to_wait = 0;
		}
		args[args_size] = NULL;

		
		pid_t pid = fork();


		if (pid < 0) {
			fprintf(stderr, "Fork fail");
			return 1;
		}
		else if (pid == 0) { // child process
			ChildProcess(args_size, args);
			exit(0);
		}
		if (need_to_wait) {
			waitpid(pid, &status, 0);
		}
	}


	return 0;
}