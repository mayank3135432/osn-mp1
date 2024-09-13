#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "constants.h"
#include "termios.h"
#include<ctype.h>

int fg(char** tokens){
    if(tokens[1]==NULL){
        fprintf(stderr, ""RED"fg: no input\n"RESET"");
        return 1;
    }
    /* if(tokens[2]!=NULL){
        fprintf(stderr, ""RED"fg: too many input"RESET"\n");
        return 1;
    } */
    for (int i=0; i<strlen(tokens[1]); i++) {
        if (!isdigit(tokens[1][i])){
            fprintf(stderr, ""RED"fg: invalid input\n"RESET"");
            return 1; // Not a numeric string
        }
    }
    
    pid_t pid = atoi(tokens[1]);
    // Check if the process exists
    if (kill(pid, 0) == -1) {
        perror(""RED"No such process found"RESET"");
        return 1;
    }
    struct termios shell_tmodes, process_tmodes;
    tcgetattr(STDIN_FILENO, &shell_tmodes);
    
    // Bring process to foreground
    tcsetpgrp(STDIN_FILENO, pid);  // Give terminal control to the process's group
    
    // Resume the process if it is stopped
    if (kill(pid, SIGCONT) == -1) {
        perror(""RED"Error continuing process"RESET"\n");
        return 1;
    }
    printf("In fid\n");
    waitpid(pid,NULL,WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    tcsetattr(STDIN_FILENO, TCSADRAIN, &shell_tmodes);
    
    /* // Wait for the process to complete or stop
    waitpid(pid, NULL, WUNTRACED);

    // Return control back to the shell
    tcsetpgrp(STDIN_FILENO, getpgrp());tcsetpgrp(STDOUT_FILENO, getpgrp()); */

    printf(""RED"Process %d brought to foreground"RESET"\n", pid);
    return 0;
}

int bg(char** tokens){
    if(tokens[1]==NULL){
        fprintf(stderr, ""RED"bg: no input\n"RESET"");
        return 1;
    }/* 
    if(tokens[2]!=NULL){
        fprintf(stderr, ""RED"bg: too many input"RESET"\n");
        return 1;
    } */
    for (int i=0; i<strlen(tokens[1]); i++) {
        if (!isdigit(tokens[1][i])){
            fprintf(stderr, ""RED"bg: invalid input\n"RESET"");
            return 1; // Not a numeric string
        }
    }
    pid_t pid = atoi(tokens[1]);
    // Check if the process exists
    if (kill(pid, 0) == -1) {
        perror(""RED"No such process found"RESET"\n");
        return 1;
    }

    // Send SIGCONT to resume the process if it is stopped
    if (kill(pid, SIGCONT) == -1) {
        perror(""RED"Error continuing process"RESET"\n");
        return 1;
    }

    printf(""YEL"Process %d resumed in the background"RESET"\n", pid);
    return 0;
}
/* 
int main(){
    bg();
} */
/* 
int fgbg(pid_t pid){
    pid_t shell_pid = getpid();
    struct dirent *entry;
    DIR *dir = opendir(PROC_DIR);
    
    if (!dir) {
        perror("Unable to open /proc directory");
        return 1;
    }
    
    ProcessInfo child_PROCESSES[MAX];
    ProcessIDetails* child_procdetails_LIST[MAX];
    

    int process_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        // Skip non-numeric directories in /proc
        if (!isdigit(entry->d_name[0])) {
            continue;
        }
        
        pid_t pid = atoi(entry->d_name);
        pid_t ppid;
        char state;
        char name[256];
        
        // Get process info for the given PID
        //if (get_process_info(pid, &ppid, &state, name) == S0) {// wut ??
        ProcessIDetails* procdetails = get_process_info(pid, &ppid, &state, name);
        if (procdetails!=NULL){ // no error
            if (ppid == shell_pid && process_count < MAX) {
                child_PROCESSES[process_count].pid = pid;
                strcpy(child_PROCESSES[process_count].name, name);
                child_PROCESSES[process_count].state = state;
                child_procdetails_LIST[process_count] = procdetails;
                process_count++;
            }
        }
    }
    
    int found = 0;


    for (int i = 0; i < process_count; i++) {
        if (child_PROCESSES[i].pid == pid) {
            found = 1;

            // Bring process to foreground
            tcsetpgrp(STDIN_FILENO, pid);  // Set terminal control to the process group

            // If the process is stopped, send SIGCONT to resume it
            
            if (child_procdetails_LIST[i]->state!='R') {
                if (kill(pid, SIGCONT) == -1) {
                    fprintf(stderr, ""RED"Error continuing process"RESET"");
                    return 1;
                }
                //child_procdetails_LIST[i]->state='R';  // Update process state to running
            }

            // Wait for the process to complete
            waitpid(pid, NULL, WUNTRACED);
            
            // Reset terminal control back to shell
            tcsetpgrp(STDIN_FILENO, getpgrp());

            printf("[fg] Process %d brought to foreground\n", pid);
            break;
        }
    }

    if (!found) {
        fprintf(stderr, ""RED"No such process found"RESET"\n");
    }
    closedir(dir);
    
    return 0;
}


 */