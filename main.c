#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * Main - function for shell.
 *
 * @return the exit status of the program
 */

int main(void)
{
	run_shell();
	return (0);
}
