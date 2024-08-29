#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "constants.h"
int hop(char** tokens, char* homedir, char* prevdir){
    if(strcmp(tokens[0],"hop") != 0){
        fprintf(stderr, ""RED"not hop command"RESET"\n");
        return 1;
    }
    if(tokens[1] == NULL){
        chdir(homedir);
    }
    int i=1;
    char cwd[MAX];
    while(tokens[i] != NULL){
        //printf("prevdir is %s\n",(prevdir==NULL)?"NULL":"NOT NULL");
        if(tokens[i][0] == '~'){ 
            char* fullname = (char*)malloc((strlen(tokens[i])+strlen(homedir))*sizeof(char));
            strcpy(fullname, homedir);
            strcat(fullname, 1 + tokens[1]);
            // printf("homedir is %s : %s\n",homedir,fullname);
            if(chdir(fullname)<0){
                fprintf(stderr, ""RED"%s: No such directory"RESET"\n",fullname);
                return 1;
            }
        }else if(tokens[i][0] == '-'){
            if(strcmp(prevdir,"NULL")==0){
                fprintf(stderr, ""RED"previous directory undefined"RESET"\n");
                return 1;
            }
            chdir(prevdir);
        }
        else{
            // printf("%s\n",tokens[i]);
            if(chdir(tokens[i])<0){
                fprintf(stderr, ""RED"%s: No such directory"RESET"\n",tokens[i]);
                return 1;
            }
        }
        
        
        i++;
    }
    
    return 0;
}