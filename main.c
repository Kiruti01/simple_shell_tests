#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * Main function for shell.
 * @return the exit status of the program
 */

int main(void)
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
