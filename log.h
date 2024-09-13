// log.h
#include "alias.h"
#include <sys/types.h>
void update_history(char* command, char* history_file);
void print_history(char* history_file);
void clear_history(char* history_file);
int is_log_command(const char* command);

int log_func(char** tokens, char* homedir, char** ptrprevdir, char* input, AliasList* aliases, pid_t shell_pid);
void execute_history_command(int index, char* homedir, char** ptrprevdir, char* input, char* history_file, AliasList* aliases, pid_t shell_pid);

