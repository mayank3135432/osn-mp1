// utils.c

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "constants.h"
#include "input.h"
char* pre_process_path(char* path, char* homedir){ // convert ~ to homedir string
    if(path[0] == '~'){ 
        char* fullname = (char*)malloc((strlen(path)+strlen(homedir))*sizeof(char));
        strcpy(fullname, homedir);
        strcat(fullname, 1 + path);
        // printf("homedir is %s : %s\n",homedir,fullname);
        /* 
        if(chdir(fullname)<0){
            fprintf(stderr, ""RED"%s: No such directory"RESET"\n",fullname);
            return 1;
        }
        */
        return fullname;
    }
    return path;

}

char* stitchtokens(char** tokens){
    char* str = (char*)malloc(MAX*sizeof(char));
    str[0]='\0';
    for(int i=0;tokens[i]!=NULL;i++){
        strcat(str, tokens[i]);
        strcat(str," ");
    }str[strlen(str)-1]=0;
    return str;
}
void printtokens(char** tokens){

    printf("the tokens are: ");
    for(int i=0;tokens[i]!=NULL;i++){
        printf("'%s',", tokens[i]);
    }
    char* str = stitchtokens(tokens);
    printf("stitched command world be is: '%s'\n", str);
    //printf("stitched newtokens will be");
}