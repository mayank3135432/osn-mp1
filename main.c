#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "constants.h"
#include "hop.h"
#include "input.h"
#include "log.h"
#include "utils.h"
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
    printf("<"GRN"%s@%s"WHT":"BLU"%s"RESET"> ",loginbuf,hostbuf,path);

    return 0;
}

void shell_loop() {
    char *input;
    char copybuf[MAX+1];
    char **args;
    char homedir[MAX];
    char* prevdir = "NULL";
    getcwd(homedir, MAX);
    char* history_file = pre_process_path(HISTORY_FILE, homedir);
    
    printf(""YEL"history file is at %s"RESET"\n", history_file);
    FILE* fptr = fopen("./.myhistory", "a");
    fclose(fptr);
    // strcpy(prevdir, homedir);
    do{
        printprompt(homedir);
        input = read_input();
        strcpy(copybuf, input);
        args = tokenise_input(input);
        if(args[0] != NULL){ // do nothing if empty input
            if(strcmp(args[0],"quit")==0) break; // command to quit shell
            update_history(copybuf, history_file);
            execute_command(args, homedir, &prevdir, copybuf); // run given command
        }
        //printf("%s\n",copybuf);
        

        
    }while(1);
    /* free(input);
    free(args); */
}

int main(){
    
    shell_loop();
    return 0;
}