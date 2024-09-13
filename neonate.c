#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>
#include "constants.h"

#define MAX_PID_LENGTH 20

int get_most_recent_pid(){
    int max_pid = 0;
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror(""RED"Error opening /proc"RESET"");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL){
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            if (pid > max_pid) {
                max_pid = pid;
            }
        }
    }

    closedir(dir);
    return max_pid;
}

// Function to check if a key has been pressed
int kbhit(){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int neonate(char** tokens){
    for(int i=1;i<=2;i++){
        if(tokens[i]==NULL){
            fprintf(stderr, RED"Usage: %s -n <time_interval>\n"RESET, tokens[0]);
            return 1;    
        }
    }
    if(tokens[3]!=NULL || strcmp(tokens[1], "-n") != 0) {
        fprintf(stderr, ""RED"Usage: %s -n <time_interval>"RESET"\n", tokens[0]);
        return 1;
    }

    int time_interval = atoi(tokens[2]);
    if (time_interval <= 0){
        fprintf(stderr, "Invalid time interval. Please provide a positive integer.\n");
        return 1;
    }

    while(1){
        int pid = get_most_recent_pid();
        if (pid != -1){
            printf("%d\n", pid);
        }

        for (int i = 0; i < time_interval; i++){
            sleep(1);
            if (kbhit()){
                char c = getchar();
                if (c == 'x' || c == 'X'){
                    printf("Exiting...\n");
                    return 0;
                }
            }
        }
    }

    return 0;
}