#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 100

/**
 * The main function of the simple shell.
 * It reads user input,
 * executes the entered commands with arguments,
 * and displays the prompt again.
 * The shell continues running
 * until the user presses Ctrl+D to exit.
 *
 * Return: Always 0.
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	char *args[MAX_COMMAND_LENGTH];
	char *token;

	while (1)
	{
		display_prompt();

		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			/* Handle the end of file condition (Ctrl+D) */
			write(1, "\n", 1);
			break;
		}

		/* Remove the trailing newline character from the input */
		command[strcspn(command, "\n")] = '\0';

		int i = 0;
		token = strtok(command, " ");
		while (token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;  /*Set the last element to NULL to terminate the args array*/

		execute_command(args[0], args);
	}

	return (0);
}
