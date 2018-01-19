#ifndef COMMAND_H
#define COMMAND_H

void execute_command(char* command, char* options, char* arguments);
int execute_builtin(int argc, char* argv[]);

#endif
