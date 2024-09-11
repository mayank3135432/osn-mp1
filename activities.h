#ifndef ACTIVITIES_H
#define ACTIVITIES_H
#include<unistd.h>

#define PROC_DIR "/proc"
#define STATUS_FILE "status"
#define MAX_LINE_LENGTH 256

// Function to get the parent process ID of a given process
int get_process_info(pid_t pid, pid_t *ppid, char *state, char *name);
// Function to get the state as a string
const char* get_state_string(char state);

int activities();

typedef struct {
    pid_t pid;
    char name[256];
    char state;
} ProcessInfo;

#endif