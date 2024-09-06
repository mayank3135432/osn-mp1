#ifndef ALIAS_H
#define ALIAS_H
#include "constants.h"

typedef struct {
    char *alias;
    char *command;
} Alias;
// I can probaably use a better data structure such as a hashtable for O(1) accsess
// but this will do for now.
typedef struct {
    Alias* element;
    int alias_count;
} AliasList;

/* Alias aliases[MAX]; // can store MAX aliases
int alias_count = 0; */

int alias(char ** tokens, AliasList* aliases);
char** use_alias(char** tokens, char* command, AliasList* aliases, int aliasnum);
    

#endif