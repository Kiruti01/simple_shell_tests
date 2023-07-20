#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tokenizer.h"

#define MAX_COMMAND_LENGTH 100

/* prototype*/
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
		char *args[MAX_COMMAND_LENGTH];
		int num_args = tokenize_command(command, args);

		/*find PATH*/
		char *cmd_path = find_command(args[0]);

		if (cmd_path != NULL)
		{
			char *envp[] = {NULL}; /* Environment variables (empty for this example) */

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

int main(void)
{
	/* store buffer */
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

		/* Remove the trailing newline, if present */
		if (characters > 0 && buff[characters - 1] == '\n')
		{
			buff[characters - 1] = '\0';
		}

		if (strlen(buff) == 0)
		{
			continue;
		}

		execute_command(buff);
	}
	/* Free mem */
	free(buff);

	return (0);
}
