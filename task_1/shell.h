#ifndef SHELL_H
#define SHELL_H

extern const char prompt[];

void display_prompt(void);
void execute_command(char *command);

#endif
