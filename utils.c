#include<stdlib.h>
#include<string.h>
char* pre_process_path(char* path, char* homedir){ // convert ~ to homedir string
    if(path[0] == '~'){ 
        char* fullname = (char*)malloc((strlen(path)+strlen(homedir))*sizeof(char));
        strcpy(fullname, homedir);
        strcat(fullname, 1 + path);
        // printf("homedir is %s : %s\n",homedir,fullname);
        /* 
        if(chdir(fullname)<0){
            fprintf(stderr, ""RED"%s: No such directory"RESET"\n",fullname);
            return 1;
        }
        */
        return fullname;
    }
    return path;

}