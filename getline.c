#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * custom_getline - read a line from stdin
 *
 * Return: A pointer to the read line or NULL if end of input reached
 */

char *custom_getline()
{
	static char buffer[BUFFER_SIZE];
	static int buffer_index;
	static int buffer_length;

	if (buffer_index == buffer_length)
	{
		buffer_length = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		if (buffer_length <= 0)
		{
			return (NULL);
		}
		buffer_index = 0;
	}

	int line_length = 0;

	while (buffer_index + line_length < buffer_length
			&& buffer[buffer_index + line_length] != '\n')
	{
		line_length++;
	}

	char *line = (char *)malloc(line_length + 1);

	if (!line)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < line_length; i++)
	{
		line[i] = buffer[buffer_index++];
	}
	line[line_length] = '\0';

	if (buffer_index < buffer_length && buffer[buffer_index] == '\n')
	{
		buffer_index++;
	}

	return (line);
}
