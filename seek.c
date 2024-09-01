/*



*/
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

#include "utils.h"
char* append_path_to_dirname(char* cwd, struct dirent *dir){
    char* path = (char*)malloc(MAX*sizeof(char));
    snprintf(path, MAX, "%s/%s", cwd, dir->d_name);
    return path;
}
// Function to implement the 'reveal' command
int reveal(char **tokens, char* homedir, char* prevdir){
    DIR *d;
    struct dirent **namelist;
    struct dirent *dir;
    struct stat file_stat;
    struct passwd *pw;
    struct group *gr;
    char* cwd = NULL;
    int only_dir = 0;
    int only_files = 0;
    int option_e = 0;
    int i = 1;

    // Parse options
    while (tokens[i] != NULL){
        tokens[i] = pre_process_path(tokens[i], homedir);
        if(strcmp(tokens[i],"-")==0){
            //printf("reveal : prevdir is %s\n",prevdir);
            cwd = prevdir;
        }
        else if(tokens[i][0] == '-'){
            if (strchr(tokens[i], 'd')) {
                only_dir = 1; // Show dotfiles
            }
            if (strchr(tokens[i], 'f')) {
                only_files = 1; // Show long format
            }
            if (strchr(tokens[i], 'f')) {
                option_e = 1; // Show long format
            }
        }
        else{
            cwd = tokens[i];
            //printf("%s\n",cwd);
        }
        i++;
    }

    if(cwd == NULL){
        // Get the current working directory
        cwd = (char*)malloc(MAX*sizeof(char));
        if (getcwd(cwd, MAX) == NULL) {
            fprintf(stderr, ""RED"could not getcwd"RESET"\n");
            return 1;
        }
    }

    // Open the current directory
    //printf("directory to b opened is %s\n",cwd);
    d = opendir(cwd);
    if (d == NULL) {
        fprintf(stderr, ""RED"could not opendir"RESET"\n");
        return 1;
    }

    
/*     int fx = fork();
    if(fx>0) return 0;
    chdir(cwd);
 */
    /* 
    int n = scandir(cwd, &namelist, NULL, alphasort);
    if (n == -1) {
        perror("scandir");
        return 1;
    } 
    */
    // Read and print each entry in the directory
    while ((dir = readdir(d)) != NULL) {
        
        // Skip dotfiles if -a is not specified
        if (!only_dir && dir->d_name[0] == '.') {
            continue;
        }

        // Print long format if -l is specified
        else {
            //char* path = append_path_to_dirname(cwd, dir);
            char* path = (strcmp(dir->d_name, "..") != 0)? append_path_to_dirname(cwd, dir):".."; // handle edge case
            if (stat(path, &file_stat) == -1) {
                fprintf(stderr, ""RED"reveal : could not stat"RESET"\n");
                continue;
            }
            if(S_ISDIR(file_stat.st_mode)){
                printf(""BLU"%s"RESET"\n", dir->d_name);
            }
            else if(file_stat.st_mode & S_IXUSR){
                printf(""GRN"%s"RESET"\n", dir->d_name);
            }
            else{
                printf("%s\n", dir->d_name);
            }
        }
    }

    // Close the directory
    closedir(d);
    return 0;
}

