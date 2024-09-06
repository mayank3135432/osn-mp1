// alias.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "alias.h"
#include "input.h"
#include "utils.h"
int alias(char ** tokens, AliasList* aliases){
    //int alias_count = aliases->alias_count;
    if(strcmp(tokens[0], "alias") !=0){
        fprintf(stderr, ""RED"not alias command"RESET"\n");
        return 1;
    }
    if (tokens[1] == NULL) {
        //printf("enter into printing\n");

        // List all aliases
        for (int i = 0; i < aliases->alias_count; i++) {
            printf("alias %s = '%s'\n", aliases->element[i].alias, aliases->element[i].command);
        }
        return 0;
    }
    if(strcmp(tokens[2],"=") != 0){
        fprintf(stderr, ""RED"alias: define using '='\n"RESET"");
        return 1;
    } 
    for (int i = 0; i < aliases->alias_count; i++) {
        if (strcmp(aliases->element[i].alias, tokens[1]) == 0) {
            
            free(aliases->element[i].command);
            char* str = (char*)malloc(MAX*sizeof(char));
            str[0]='\0';
            for(int i=3;tokens[i]!=NULL;i++){
                strcat(str, tokens[i]);
                strcat(str," ");
            }str[strlen(str)-1]=0;


            aliases->element[i].command = strdup(str);

            free(str);
            return 1;
        }
    }
    if(aliases->alias_count<MAX){
        char* str = (char*)malloc(MAX*sizeof(char));
        str[0]='\0';
        for(int i=3;tokens[i]!=NULL;i++){
            strcat(str, tokens[i]);
            strcat(str," ");
        }str[strlen(str)-1]=0;
        aliases->element[aliases->alias_count].alias = strdup(tokens[1]);
        aliases->element[aliases->alias_count].command = strdup(str);
        aliases->alias_count++;
        free(str);
    }else{
        fprintf(stderr, ""RED"alias: limit reached"RESET"");
        return 1;
    }
    return 0;
}

char** use_alias(char** tokens, char* command, AliasList* aliases, int aliasnum){
    //printtokens(tokens);
    char* aliascommand = strdup(aliases->element[aliasnum].command);
    tokens[0] = aliascommand;
    //printtokens(tokens);
    char* str = stitchtokens(tokens);
    char** newtokens = tokenise_input(str);
    return newtokens;
}




