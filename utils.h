// utils.h
char* stitchtokens(char** tokens);
char* pre_process_path(char* path, char* homedir);
void printtokens(char** tokens);
void printlistoftokens(char*** list);
void free_split_tokens(char*** list);