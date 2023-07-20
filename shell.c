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
	char *cmd_path;
	char *token = strtok(command, " ");

	if (token == NULL)
	{
		/*empty cmnd do nothing*/
		return;
	}

	/*exit code*/
	if (strcmp(token, "exit") == 0)
	{
		char exit_msg[] = "logging out..\n";
		write(STDOUT_FILENO, exit_msg, sizeof(exit_msg) -1);
		exit(0);
	}

	/*find command path*/
	cmd_path = find_command(token);

	if (cmd_path != NULL)
	{
		pid_t pid = fork();

		if (pid == 0)
		{
			/* Split the command into tokens for execve */
			char *envp[] = {NULL};
			/* Environment variables (empty for this example)*/

			execve(cmd_path, &command, envp);
			perror("Error: Command execution failed");
			free(cmd_path);
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

		free(cmd_path);
	}
	else
	{
		perror("Error: command not found in PATH\n");
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
