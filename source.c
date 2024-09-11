/*
Modified code initially generated by GPT
https://chatgpt.com/share/449aa3ab-08df-4d2d-b10d-33886207b543 - the last prompt
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "alias.h"
#include "input.h"
#include "utils.h"

int source_myshrc(char *homedir, AliasList* aliases){
    char* myshrc_path;
    char *line = NULL;
    unsigned long len = 0;
    int read;
    char** tokens;
    myshrc_path = pre_process_path("~/.myshrc", homedir);
    

    FILE *file = fopen(myshrc_path, "r");
    if (file == NULL) {
        printf("No .myshrc file found.\n");
        return 1;
    }
    
    while ((read = getline(&line, &len, file)) != -1) {
        
        tokens = tokenise_input(line); // problematic
        // If the line is not empty
        if (tokens[0] != NULL) {
            execute_command(tokens, homedir, NULL, line, aliases, 1);
        }
        
        free(tokens);
    }

    fclose(file);
    if (line) free(line);

    return 0;
}
