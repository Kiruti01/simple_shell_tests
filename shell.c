#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		/* Split the command into tokens for execve */
		char *args[MAX_COMMAND_LENGTH];
		int i = 0;
		char *token = strtok(command, " ");

		while (token != NULL && i < MAX_COMMAND_LENGTH - 1)
		{
			args[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;

		char *envp[] = {NULL}; /* Environment variables (empty for this example)*/

		execve(args[0], args, envp);
		perror("Error: Command not found");
		_exit(1);
	}
	else if (pid < 0)
	{
		perror("Error: Fork failed");
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
