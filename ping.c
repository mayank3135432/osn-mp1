#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "constants.h"

void ping_util(int pid, int signal_input) {
    int signal = signal_input % 32;

    if(kill(pid, signal) == -1){
        if(errno == ESRCH){
            fprintf(stderr, ""RED"No such process found"RESET"\n");
        }else{
            fprintf(stderr, ""RED"Failed to send signal %d to process with pid %d: %s"RESET"\n", signal, pid, strerror(errno));
        }
    }else{
        printf(""BRIGHTBLU"Sent signal %d to process with pid %d"RESET"\n", signal, pid);
    }
}
int ping(char** input){
    if(input[1]==NULL || input[2]==NULL){
        fprintf(stderr, ""RED"ping: arguments not of form <pid> <signal_number>\n"RESET"");
        return 1;
    }
    int pid = atoi(input[1]);
    int signal_number = atoi(input[2]);
    ping_util(pid, signal_number);

    return 0;
}