#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "alias.h"
#include "input.h"
int main(){
    printf("hello\n");
    char* input = read_input();
    char** tokens = tokenise_input(input);
    //char** tokens = tokenise_input("alias reveall = reveal -l  -a  -la");
    char* str = (char*)malloc(MAX*sizeof(char));
    str[0]='\0';
    for(int i=3;tokens[i]!=NULL;i++){
        strcat(str, tokens[i]);
        strcat(str," ");
    }str[strlen(str)-1]=0;

    printf("%shello\nfff", str);
    free(str);
    return 0;
}
