#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

void display_prompt(void);
void execute_command(char *command, char *args[]);

#endif
