#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

void display_prompt(void);
void execute_command(char *command, char *args[]);
char *custom_getline(void);
void execute_in_current_dir(char *command, char *args[]);
void execute_in_path(char *command, char *args[]);
void redirect_io(void);
int tokenize_input(char *command, char *args[]);
int set_environment_variable(char *variable, char *value);
int unset_environment_variable(char *variable);

#endif
