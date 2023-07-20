#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 100

/* Define prompt as a global variable*/
const char prompt[] = "My_shell :)$ ";

/**
  * display_prompt - Displays the shell prompt.
  */
void display_prompt(void)
{
 	write(STDOUT_FILENO, prompt, strlen(prompt));
}

/**
 * execute_command - Executes the given command with arguments using execve.
 * @command: The command to execute.
 * @args: The array of arguments for the command.
 */
void execute_command(char *command, char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("Fork failed.");
		return;
	}
	else if (pid == 0)
	{
		/* Child process */
		char path[] = "/bin/";

		strcat(path, command);

		execve(path, args, NULL);
		perror("Command execution failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		int status;
		wait(&status);
	}
}
