#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

/**
 * tokenize_input - Tokenizes user input into arguments.
 * @command: The user input.
 * @args: Array to store the tokenized arguments.
 *
 * Return: The number of arguments tokenized.
 */

int tokenize_input(char *command, char *args[])
{
	char *token;
	int i = 0;

	token = strtok(command, " ");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	return (i);
}

/**
 * main - The main function of the simple shell.
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
	bool should_exit = false; /*add variable to handle exit*/

	while (!should_exit)
	{
		display_prompt();

		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			/* Handle the end of file condition (Ctrl+D) */
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		/* Remove the trailing newline character from the input */

		command[strcspn(command, "\n")] = '\0';

		int num_args = tokenize_input(command, args);

		if (num_args == 0)
		{
			continue;/* ignore empty cmds*/
		}

		/*handle built in cmds*/
		if (strcmp(args[0], "exit") == 0)
		{
			should_exit = true;
		}
		else if (strcmp(args[0], "setenv") == 0)
		{
			if (num_args != 3)
			{
				const char *error_msg = "Usage: setenv VARIABLE VALUE\n";
				write(STDERR_FILENO, error_msg, strlen(error_msg));
			}
			else
			{
				if (set_environment_variable(args[1], args[2]) == -1)
				{
					const char *error_msg = "Failed to set environment variable\n";
					write(STDERR_FILENO, error_msg, strlen(error_msg));
				}
			}
		}
		else if (strcmp(args[0], "unsetenv") == 0)
		{
			if (num_args != 2)
			{
				const char *error_msg = "Usage: unsetenv VARIABLE\n";
				write(STDERR_FILENO, error_msg, strlen(error_msg));
			}
			else
			{
				if (unset_environment_variable(args[1]) == -1)
				{
					const char *error_msg = "Failed to unset environment variable\n";
					write(STDERR_FILENO, error_msg, strlen(error_msg));
				}
			}
		}
		else
		{
			execute_command(args[0], args);
		}
	}
	return (0);
}
