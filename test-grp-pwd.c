#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "constants.h"
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include "utils.h"

int main(){
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    struct passwd *pw;
    struct group *gr;
    char path[MAX] = "/home/ateium/ShanghaiAlicee";
    d = opendir(path);
    if (stat(path, &file_stat) == -1) {
        fprintf(stderr, ""RED"could not stat"RESET"\n");
    }
    
    pw = getpwuid(file_stat.st_uid);
    errno=0;
    gr = getgrgid(file_stat.st_gid);
    printf("error number : %d\n",errno);
    printf("user name is %s\n",pw->pw_name);
    printf("group name is %s\n",gr!=NULL?gr->gr_name:""RED"NULL"RESET"");

    return 0;
}