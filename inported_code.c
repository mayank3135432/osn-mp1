#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100
#define MAX_PIPES 10

void execute_command(char *command);

void parse_and_execute(char *input) {
    char *commands[MAX_PIPES];  // To store commands split by pipes
    int num_commands = 0;

    // Split the input by pipes
    char *pipe_token = strtok(input, "|");
    while (pipe_token != NULL) {
        commands[num_commands++] = pipe_token;
        pipe_token = strtok(NULL, "|");
    }

    int pipe_fds[2 * (num_commands - 1)]; // Array for pipe file descriptors

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipe_fds + 2 * i) < 0) {
            perror("pipe failed");
            exit(1);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        char *args[MAX_ARGS];
        int arg_count = 0;
        int input_fd = -1, output_fd = -1;

        // Split command into arguments
        char *arg_token = strtok(commands[i], " ");
        while (arg_token != NULL) {
            if (strcmp(arg_token, "<") == 0) {
                // Input redirection
                arg_token = strtok(NULL, " ");
                input_fd = open(arg_token, O_RDONLY);
                if (input_fd < 0) {
                    perror("No such input file found!");
                    return;
                }
            } else if (strcmp(arg_token, ">") == 0) {
                // Output redirection (overwrite)
                arg_token = strtok(NULL, " ");
                output_fd = open(arg_token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else if (strcmp(arg_token, ">>") == 0) {
                // Output redirection (append)
                arg_token = strtok(NULL, " ");
                output_fd = open(arg_token, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                args[arg_count++] = arg_token;
            }
            arg_token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            // Child process

            // Handle input redirection
            if (input_fd != -1) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Handle output redirection
            if (output_fd != -1) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            // Handle pipes
            if (i > 0) {
                // If not the first command, get input from previous pipe
                dup2(pipe_fds[2 * (i - 1)], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                // If not the last command, output to the next pipe
                dup2(pipe_fds[2 * i + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < 2 * (num_commands - 1); j++) {
                close(pipe_fds[j]);
            }

            execvp(args[0], args); // Execute the command
            perror("exec failed");
            exit(1);
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }

    // Parent process closes all pipe file descriptors
    for (int i = 0; i < 2 * (num_commands - 1); i++) {
        close(pipe_fds[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

int main() {
    char command[1024];

    while (1) {
        printf("<JohnDoe@SYS:~> ");
        fflush(stdout);
        
        if (!fgets(command, sizeof(command), stdin)) {
            break; // Exit on EOF
        }
        
        command[strcspn(command, "\n")] = 0; // Remove newline character
        
        if (strcmp(command, "exit") == 0) {
            break; // Exit shell
        }

        parse_and_execute(command);
    }

    return 0;
}
