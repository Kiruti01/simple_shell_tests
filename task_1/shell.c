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
 */

void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("Fork failed.");
		return;
	}
	else if (pid == 0)
	{
		/*Child process*/
		char *args[] = {command, NULL};
		char path[] = "/bin/";

		strcat(path, command);  /* Full path to the command*/

		char *envp[] = {NULL}; /* Array of environment variables*/

		execve(path, args, envp);
		perror("Command execution failed");
		exit(EXIT_FAILURE);
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

		execute_command(command);
	}

	return (0);
}
