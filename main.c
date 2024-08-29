#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "constants.h"
#include "hop.h"
#include "input.h"
int printprompt(char* homedirbuf){
    char cwdbuf[MAX];
    char loginbuf[MAX];
    char hostbuf[MAX];
    getcwd(cwdbuf, MAX);
    getlogin_r(loginbuf, MAX);
    gethostname(hostbuf, MAX);
    char* path;
    if(!strncmp(homedirbuf, cwdbuf, strlen(homedirbuf))){
        if(!strcmp(homedirbuf,cwdbuf)){
            path="~";
        }
        else{
            path = cwdbuf + (strlen(homedirbuf)-1);
            path[0]='~';
        }
    }
    else{
        path = cwdbuf;
    }
    printf("<"GRN"%s@%s"WHT":"BLU"%s"RESET">",loginbuf,hostbuf,path);

    return 0;
}

void shell_loop() {
    char *input;
    char **args;
    int status = 1;
    char homedir[MAX];
    char* prevdir = "NULL";
    getcwd(homedir, MAX);
    // strcpy(prevdir, homedir);
    do{
        printprompt(homedir);
        input = read_input();
        args = tokenise_input(input);
        status = execute_command(args, homedir, &prevdir);

        /* free(input);
        free(args); */
    }while(1);
}

int main(){
    shell_loop();
    /* char homedir[MAX];
    getcwd(homedir, MAX);
    printprompt(homedir);
    char* X = read_input();
    char** tokens = tokenise_input(X);
    for(int i=0;tokens[i]!=NULL;i++){
        printf("%s,",tokens[i]);
    }printf("\n");
    hop(tokens);
    printprompt(homedir); */

    return 0;
}