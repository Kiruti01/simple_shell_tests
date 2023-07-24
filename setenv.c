#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * set_environment_variable - Initializes or modifies an environment variable.
 * @variable: The name of the environment variable.
 * @value: The value to set for the environment variable.
 *
 * Return: 0 on success, -1 on failure.
 */

int set_environment_variable(char *variable, char *value)
{
	if (variable == NULL || value == NULL)
	{
		const char *error_msg = "Setenv: Invalid argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return (-1);
	}

	if (setenv(variable, value, 1) != 0)
	{
		const char *error_msg = "Setenv: Failed to set environment variable\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return (-1);
	}
	return (0);
}

