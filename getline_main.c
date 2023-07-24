#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char *line;

	while (1)
	{
		line = custom_getline();

		if (line == NULL)
		{
			/*end of input or error exit loop*/
			break;
		}

		/*process the line as needed*/
		write(STDOUT_FILENO, "You entered: ", 13);
		write(STDOUT_FILENO, line, strlen(line));
		write(STDOUT_FILENO, "\n", 1);

		free(line);
	}

	return (0);
}
