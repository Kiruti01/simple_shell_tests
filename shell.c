#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100 

/* finds command in path*/
static char *find_command(char *command)
{
	char *path = getenv("PATH");
	if (path == NULL)
	{
		write(STDERR_FILENO, "Error: Path variable not found\n", 31);
		return NULL;
	}

	char *path_copy = strdup(path);
	if (path_copy == NULL)
	{
		write(STDERR_FILENO, "Error: strdup failed\n", 21);
		return NULL;
	}

	char *dir = strtok(path_copy, ":");
	char *cmd_path = (char *)malloc(strlen(command) + 256);

	while (dir != NULL)
	{
		snprintf(cmd_path, strlen(command) + 256, "%s/%s", dir, command);
		if (access(cmd_path, X_OK) == 0)
		{
			free(path_copy);
			return cmd_path;
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	free(cmd_path);
	return NULL;
}

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
		char *args[MAX_ARGS + 1];
		int i = 0;
		char *token = strtok(command, " ");

		while (token != NULL && i < MAX_ARGS)
		{
			args[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;

		char *cmd_path = find_command(args[0]);

		if (cmd_path != NULL)
		{
			execve(cmd_path, args, NULL);
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
			/*check for end of file*/
			if (feof(stdin))
			{
				break;
			}
			else
			{
				perror("Error: getline fail");
				exit(1);
			}
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
