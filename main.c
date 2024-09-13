// main.c
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
#include "alias.h"
#include "source.h"
//#include "interrupts.h"



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
    fprintf(stdout, "<"GRN"%s@%s"WHT":"BLU"%s"RESET"> ",loginbuf,hostbuf,path);


    return 0;
}


void handle_sigint(pid_t shell_pid){
    int fg_pid = getpgid(getpid());
    if (fg_pid!=shell_pid){
        printf("\n>");
        return;
    }
    /* int fg_pid = getpid();
    if (fg_pid!=shell_pid){
        // Send SIGINT to the foreground process
        printf("kill\n");
        kill(fg_pid, SIGINT);
    } else {
        // If no foreground process, ignore SIGINT in shell
        printf("\n");
    } */
}



void shell_loop(AliasList* aliases) {
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
    pid_t shell_pid = getpid();

    
    source_myshrc(homedir, aliases, shell_pid); // problem
    // strcpy(prevdir, homedir);

    struct sigaction sa2;
    sa2.sa_handler = handle_sigint;
    sa2.sa_flags = SA_RESTART;
    sigemptyset(&sa2.sa_mask);
    if (sigaction(SIGINT, &sa2, NULL) == -1) {
        perror(RED"ERROR IN HANDLING SIGNALS\n"RESET);
        exit(EXIT_FAILURE);
    }

    do{
        
        printprompt(homedir);
        input = read_input();
        
        strcpy(copybuf, input);
        args = tokenise_input(input);
        if(args[0] != NULL){ // do nothing if empty input
            if(strcmp(args[0],"exit")==0) return; // command to quit shell
            update_history(copybuf, history_file);
            RUN_command(args, homedir, &prevdir, copybuf, aliases, 1, shell_pid); // run given command
        }
        //printf("%s\n",copybuf);
        free(input);
        free(args);
        
    }while(1);
}

int main(){
    
    AliasList* aliases = (AliasList*)malloc(1*sizeof(AliasList));
    aliases->element = (Alias*)malloc(MAX*sizeof(Alias));
    aliases->alias_count=0;
    shell_loop(aliases);
    return 0;
}

