#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "input.h"
#include "constants.h"
#include "utils.h"
#include "pipe.h"

void handle_sigchld2() {
    // Wait for all dead child processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int handle_redirection(char **tokens){
    int input_fd = -1, output_fd = -1, append_fd = -1;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], ">") == 0) {
            output_fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) {
                perror(""RED"Error opening output file"RESET"");
                return -1;
            }
            dup2(output_fd, STDOUT_FILENO);  // Redirect stdout
            close(output_fd);
            tokens[i] = NULL;  // Nullify redirection operator
        }
        else if (strcmp(tokens[i], ">>") == 0) {
            append_fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (append_fd < 0) {
                perror(""RED"Error opening append file"RESET"");
                return -1;
            }
            dup2(append_fd, STDOUT_FILENO);  // Redirect stdout for appending
            close(append_fd);
            tokens[i] = NULL;
        }
        else if (strcmp(tokens[i], "<") == 0) {
            input_fd = open(tokens[i+1], O_RDONLY);
            if (input_fd < 0) {
                fprintf(stderr, RED"No such input file found!\n"RESET);
                return -1;
            }
            dup2(input_fd, STDIN_FILENO);  // Redirect stdin
            close(input_fd);
            tokens[i] = NULL;
        }
    }
    return 0;
}

int execute_piped_commands(char** tokens, char* homedir, char** prevdir, char* input, AliasList* aliases, int expand_alias_flag, pid_t shell_pid){
    int pipefds[2];
    pid_t pid;
    int infd=0;
    char*** commands = split_tokens(tokens);
    for (int i=0; commands[i]!=NULL; i++){
        //if this is not the last command then create a pipe
        if (commands[i+1] != NULL){
            if (pipe(pipefds) == -1){
                perror(""RED"pipe"RESET"");
                exit(1);
            }
        }


        struct sigaction sa;
        sa.sa_handler = &handle_sigchld2;
        sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror(""RED"sigaction"RESET"");
            exit(EXIT_FAILURE);
        }

        if ((pid = fork()) == -1){
            perror(""RED"fork"RESET"");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
        int ERR;
            if(infd != 0){
                dup2(infd, STDIN_FILENO);
                close(infd);
            }

            if(commands[i+1] != NULL){
                dup2(pipefds[1], STDOUT_FILENO);
                close(pipefds[0]);
                close(pipefds[1]);
            }
            //printtokens(commands[i]);
            if((ERR = execute_command(commands[i], homedir, prevdir, input, aliases, expand_alias_flag, shell_pid)) == 1){
                fprintf(stderr, ""RED"pipe: execute_command error"RESET"");
            }
            exit(ERR); // kill the child
        }else{ // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (commands[i+1] != NULL){
                close(pipefds[1]);
            }
            infd = pipefds[0];
        }
    }
}

int there_are_pipes(char** tokens){
    int flag=0;
    for(int i=0;tokens[i]!=NULL;i++){
        if(strcmp(tokens[i], "|")==0){
            if(tokens[i+1]==NULL){
                
                return -1;
            }
            flag = 1;
        }
    }
    return flag;
}

int there_is_redirec(char** tokens){
    for(int i=0;tokens[i]!=NULL;i++){
        if(!strcmp(tokens[i], ">") || !strcmp(tokens[i], ">>") || !strcmp(tokens[i], "<")){
            if(tokens[i+1]==NULL){
                return -2;
            }
            return(i);
        }
    }
    return 0;
}
char*** split_tokens(char** tokens){
    int i,j=0;
    for(i=0;tokens[i]!=NULL;i++);
    int size=i+1;
    char*** list = (char***)malloc(size*sizeof(char**));
    for(int a=0;a<size;a++){
        list[a] = (char**)malloc(size*sizeof(char*));
    }
    for(int a=0;a<size;a++){
        for(int b=0;b<size;b++){
            list[a][b] = (char*)malloc(MAX*sizeof(char));
            list[a][b][0] = 0;
        }
    }
    int b=0;
    int a=0;
    for(i=0;tokens[i]!=NULL;i++){
        //printf("(%d,%d)\n",a,b);
        /* if(tokens[i]==NULL){
            list[a][b]=NULL;
        } */
        if(strcmp(tokens[i],"|")==0){
            list[a][b]=NULL;
            a++;b=0;
        }else{
            strcpy(list[a][b], tokens[i]);
            b++;
        }
    }list[a][b]=NULL;
    list[++a]=NULL;


    return(list);

}