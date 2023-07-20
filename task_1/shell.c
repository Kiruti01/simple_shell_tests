#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 100

/*Define prompt as a global variable*/
const char prompt[] = "My_shell :)$ ";

/**
 * display_prompt - Displays shell prompt
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
	pid_t pid = fork();

	if (pid < 0)
	{
		write(STDERR_FILENO, "Fork Failed.\n", 13);
		return;
	}
	else if (pid == 0)
	{
		/*Child process*/
		/* check if cmd is '/bin/ls'*/
		if (strcmp(command, "/bin/ls")  == 0)
		{
			execve(command, args, NULL);
		}
		else
		{
			char error_msg[100];
			snprintf(error_msg, sizeof(error_msg), "Command not found: %s\n", command);
			write(STDERR_FILENO, error_msg, strlen(error_msg));
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process*/
		int status;

		wait(&status);
	}
}

/**
 * main - The main function of the simple shell.
 * It reads user input,
 * executes the entered commands,
 * and displays the prompt again.
 * The shell continues running
 * until the user presses Ctrl+D to exit.
 *
 * Return: Always 0
 */

int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();

		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			/* Handle the end of file condition (Ctrl+D)*/
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove the trailing newline character from the input*/
		command[strcspn(command, "\n")] = '\0';

		/*slit into arguments*/
		char *args[MAX_COMMAND_LENGTH];
		char *token;
		int i = 0;
		token = strtok(command, " ");
		while (token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;

		execute_command(args[0], args);
	}

	return (0);
}
