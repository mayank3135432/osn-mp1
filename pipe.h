#include "alias.h"
#include <sys/types.h>
int there_are_pipes(char** tokens);
char*** split_tokens(char** tokens);
int there_is_redirec(char** tokens);
int handle_redirection(char **tokens);
int execute_piped_commands(char** tokens, char* homedir, char** prevdir, char* input, AliasList* aliases, int expand_alias_flag, pid_t shell_pid);