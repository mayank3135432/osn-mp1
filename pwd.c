#include <stdio.h>
#include <unistd.h>
#include "constants.h"
int pwd(){
    char cwdbuf[MAX];
    char loginbuf[MAX];
    char hostbuf[MAX];
    getcwd(cwdbuf, MAX);
    printf("%s", cwdbuf);
    return 0;
}