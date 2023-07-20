#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* finds command inputed*/
char *find_command(char *command);

/**
 * Execute user commands in child process using execve
 * prints error message if cmd not found
 *
 * @param command the command to be executed
 */

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

		char *envp[] = {NULL};/* Environment variables (empty for this example)*/

		char *cmd_path = find_command(args[0]);

		if (cmd_path != NULL)
		{
			execve(cmd_path, args, envp);
			perror("Error: Command execution failed");
			free(cmd_path);
			_exit(1);
		}
		else
		{
			perror("Error: Command not found in PATH\n");
			free(cmd_path);
			_exit(1);
		}
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

void run_shell()
{
	char *buff = NULL;
	size_t buff_size = 0;
	ssize_t characters;

	while (1)
	{
		write(STDOUT_FILENO, "$ ", 2);

		characters = getline(&buff, &buff_size, stdin);

		if (characters == -1)
		{
			break;
		}

		if (strlen(buff) > 0 && buff[characters -1] == '\n')
		{
			buff[characters - 1] = '\0';
		}

		if (strlen(buff) == 0)
		{
			continue;
		}

		execute_command(buff);
	}

	free(buff);
}
