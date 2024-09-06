// seek.h

void print_path_with_color(const char* path, int is_dir);
void recursive_seek(const char* base_path, const char* target, int look_for_files, int look_for_dirs, char** found_path, int* found_count);
int seek(char** tokens, char* homedir);