/* Working Well */
#include <fcntl.h>
#include<stdio.h>
#include<string.h>
#include <threads.h>
#include<unistd.h>
#include "constants.h"

/* <Username@SystemName:~> */

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
/* int cd(char* destdir){
    return(chdir(destdir));
} */
/* int main(){
    char homedir[MAX];
    getcwd(homedir, MAX);
    printprompt(homedir);
    printf("%d\n",chdir("../.."));
    printprompt(homedir);

    //creat("newfile.txt", 0644);

    return 0;
} */