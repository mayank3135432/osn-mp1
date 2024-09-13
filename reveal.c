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

char* append_path_to_dirname(char* cwd, const char *name){
    char* path = (char*)malloc(MAX*sizeof(char));
    snprintf(path, MAX, "%s/%s", cwd, name);
    return path;
}

int compare_entries(const struct dirent **a, const struct dirent **b){
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void print_file_info(const char *path, const char *name, int show_long){
    struct stat file_stat;
    
    if(lstat(path, &file_stat) == -1){
        fprintf(stderr, RED"could not stat %s: %s"RESET"\n", path, strerror(errno));
        return;
    }

    if(show_long){
        //printf("before segma cwd/dir->d_name is %s/%s\n",cwd,dir->d_name);
            // edge case when dir->d_name is ..
            // causes segma
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);
        char *time_str = ctime(&file_stat.st_mtime);
        time_str[strlen(time_str) - 1] = '\0';

        printf("%c%c%c%c%c%c%c%c%c%c %ld %s %s %ld %s ",
               S_ISDIR(file_stat.st_mode) ? 'd' : (S_ISLNK(file_stat.st_mode) ? 'l' : '-'),
               (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
               (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
               (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
               (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
               (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
               (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
               (file_stat.st_mode & S_IROTH) ? 'r' : '-',
               (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
               (file_stat.st_mode & S_IXOTH) ? 'x' : '-',
               file_stat.st_nlink,
               pw ? pw->pw_name : "unknown",
               gr ? gr->gr_name : "unknown",
               file_stat.st_size,
               time_str
        );
    }

    if (S_ISDIR(file_stat.st_mode)){
        printf(BLU"%s"RESET, name);
    }else if (S_ISLNK(file_stat.st_mode)){
        char linkname[MAX];
        ssize_t len = readlink(path, linkname, sizeof(linkname)-1);
        if(len != -1){
            linkname[len] = '\0';
            printf(CYN"%s"RESET" -> %s", name, linkname);
        }else{
            printf(CYN"%s"RESET, name);
        }
    } else if(file_stat.st_mode & S_IXUSR){
        printf(GRN"%s"RESET, name);
    }else{
        printf("%s", name);
    }
    printf("\n");
}

int reveal(char **tokens, char* homedir, char* prevdir){
    struct dirent **namelist = NULL;
    char* cwd = NULL;
    int show_all = 0;
    int show_long = 0;
    int i = 1;
    char *target = NULL;

    // Parse options and get target
    while (tokens[i] != NULL) {
        if (strcmp(tokens[i], "-") == 0) {
            target = prevdir;
        } else if (tokens[i][0] == '-') {
            if (strchr(tokens[i], 'a')) show_all = 1;
            if (strchr(tokens[i], 'l')) show_long = 1;
        } else if (!target) {
            target = tokens[i];
        }
        i++;
    }

    if (!target) {
        cwd = (char*)malloc(MAX*sizeof(char));
        if (getcwd(cwd, MAX) == NULL) {
            fprintf(stderr, RED"could not getcwd"RESET"\n");
            return 1;
        }
        target = cwd;
    }

    target = pre_process_path(target, homedir);

    struct stat st;
    if (lstat(target, &st) == -1) {
        fprintf(stderr, RED"could not access %s: %s"RESET"\n", target, strerror(errno));
        if (cwd) free(cwd);
        return 1;
    }

    if (S_ISDIR(st.st_mode)) {
        // Target is a directory, list its contents
        int n = scandir(target, &namelist, NULL, compare_entries);
        if (n == -1) {
            fprintf(stderr, RED"could not scan directory %s: %s"RESET"\n", target, strerror(errno));
            if (cwd) free(cwd);
            return 1;
        }

        for (i = 0; i < n; i++) {
            if (!show_all && namelist[i]->d_name[0] == '.'){
                free(namelist[i]);
                continue;
            }
            char* path = append_path_to_dirname(target, namelist[i]->d_name);
            print_file_info(path, namelist[i]->d_name, show_long);
            free(path);
            free(namelist[i]);
        }
        free(namelist);
    }else{
        // Target is a file, just print its info
        print_file_info(target, target, show_long);
    }

    if (cwd) free(cwd);
    return 0;
}