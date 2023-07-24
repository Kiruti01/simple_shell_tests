#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100

/* Define prompt as a global variable*/
const char prompt[] = "My_shell :)$ ";

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
	write(STDOUT_FILENO, prompt, strlen(prompt));
}

/**
 * execute_command - Executes given cmd using execve
 * @command: the cmd to execute
 * @args: array of argumnts for cmd
 */

void execute_command(char *command, char *args[])
{
	if (access(command, X_OK) == 0)
	{
		 /* The command exists and is executable in the current directory */
		execute_in_current_dir(command, args);
	}
	else
	{
		/* Command not found in the current directory, try the PATH */
		execute_in_path(command, args);
	}
}

/**
 * execute_in_current_dir - Exectes comd if in current dir
 * @command: the cmd to execute
 * @args: array of arguments for cmd
 */

void execute_in_current_dir(char *command, char *args[])
{
	/* The command exists and is executable in the current directory */
	pid_t pid = fork();

	if (pid < 0)
	{
		write(STDERR_FILENO, "Fork Failed.\n", 13);
		return;
	}
	else if (pid == 0)
	{
		/* Child process */
		/* Redirect standard input, output, and error to the corresponding streams*/
		redirect_io();
		execve(command, args, NULL);

		char error_msg[100];

		snprintf(error_msg, sizeof(error_msg), "Execution failed: %s\n", command);
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		_exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process*/
		int status;

		wait(&status);
	}
}

/**
 * execute_in_path - Executes the command if it's in the PATH
 * @command: the cmd to execute
 * @args: array of arguments for cmd
 */
void execute_in_path(char *command, char *args[])
{
	char *path = getenv("PATH");

	if (path != NULL)
	{
		char *path_copy = strdup(path);
		char *dir = strtok(path_copy, ":");

		while (dir != NULL)
		{
			char *full_path = malloc(strlen(dir) + strlen(command) + 2);

			sprintf(full_path, "%s/%s", dir, command);

			if (access(full_path, X_OK) == 0)
			{
				/* The command exists and is executable in this directory */
				pid_t pid = fork();

				if (pid < 0)
				{
					write(STDERR_FILENO, "Fork Failed.\n", 13);
					free(full_path);
					free(path_copy);
					return;
				}
				else if (pid == 0)
				{
					/* Child process */
					execve(full_path, args, NULL);

					char error_msg[100];

					snprintf(error_msg, sizeof(error_msg), "Execution failed: %s\n", command);
					write(STDERR_FILENO, error_msg, strlen(error_msg));
					free(full_path);
					free(path_copy);
					_exit(EXIT_FAILURE);
				}
				else
				{
					/* Parent process */
					int status;

					wait(&status);
					free(full_path);
					free(path_copy);
					return;
				}
			}

			free(full_path);
			dir = strtok(NULL, ":");
		}

		free(path_copy);
	}

	char error_msg[100];

	snprintf(error_msg, sizeof(error_msg), "Command not found: %s\n", command);
	write(STDERR_FILENO, error_msg, strlen(error_msg));
}

/**
 * redirect_io - Redirects standard input, output, and error to the corresponding streams
 */

void redirect_io(void)
{
	int input_fd = open("/dev/null", O_RDONLY);

	if (input_fd < 0)
	{
		perror("Error opening /dev/null");
		exit(EXIT_FAILURE);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);

	int output_fd = open("/dev/tty", O_WRONLY);

	if (output_fd < 0)
	{
		perror("Error opening terminal");
		exit(EXIT_FAILURE);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	int error_fd = open("/dev/tty", O_WRONLY);

	if (error_fd < 0)
	{
		perror("Error opening terminal");
		exit(EXIT_FAILURE);
	}
	dup2(error_fd, STDERR_FILENO);
	close(error_fd);
}
