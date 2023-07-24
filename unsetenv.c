#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * unset_environment_variable - Removes an environment variable.
 * @variable: The name of the environment variable to remove.
 *
 * Return: 0 on success, -1 on failure.
 */

int unset_environment_variable(char *variable)
{
	if (variable == NULL)
	{
		const char *error_msg = "Unsetenv: Invalid argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return (-1);
	}

	if (unsetenv(variable) != 0)
	{
		const char *error_msg = "Unsetenv: Failed to unset environment variable\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return (-1);
	}
	return (0);
}
