#ifndef ACTIVITIES_H
#define ACTIVITIES_H
#include<unistd.h>

#define PROC_DIR "/proc"
#define STATUS_FILE "status"
//#define MAX_LINE_LENGTH 4096


typedef struct {
    pid_t pid;
    char name[256];
    char state;
} ProcessInfo;

typedef struct {
    pid_t pid;
    pid_t ppid;
    char state;
    char* name;
} ProcessIDetails;
//ProcessIDetails S0={0,0,0,NULL};
ProcessIDetails* get_process_info(pid_t pid, pid_t *ppid, char *state, char *name);
const char* get_state_string(char state);
int activities();


#endif