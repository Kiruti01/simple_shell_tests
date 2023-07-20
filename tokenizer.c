#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_ARGS 50

/**
 * find command in PATH
 */

char *find_command(char *command)
{
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *dir = strtok(path_copy, ":");
	char *cmd_path = (char *)malloc(strlen(command) + 256);

	while (dir != NULL)
	{
		snprintf(cmd_path, strlen(command) + 256, "%s/%s", dir, command);
		if (access(cmd_path, X_OK) == 0)
		{
			free(path_copy);
			return cmd_path;
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	free(cmd_path);
	return NULL;
}

/**
 * Tokenize the command and its arguments
 * @param command the command to be tokenized
 * @param args the array to store the command and its arguments
 * @return the number of arguments
 */

int tokenize_command(char *command, char *args[])
{
	int i = 0;
	char *token = strtok(command, " ");

	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;

	return i;
}	
