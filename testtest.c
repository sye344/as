#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char* kash_read_line() {
    char *line = NULL;
    size_t buflen = 0;
    getline(&line, &buflen, stdin);
    return line;
}

char** kash_split_line(char *line) {
    int length = 0;
    int capacity = 16;
    char **tokens = malloc(capacity * sizeof(char*));

    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters);

    while (token != NULL) {
        tokens[length] = token;
        length++;

        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
        }

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;
    return tokens;
}


void kash_exec(char **args) {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        execvp(args[0], args);
        perror("kash");
        exit(1);
    } else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("kash");
    }
}
int main() {
    while (true) {
        printf("> ");
        char *line = kash_read_line();
        char **tokens = kash_split_line(line);

        if (tokens[0] != NULL) {
            kash_exec(tokens);
        }

        free(tokens);
        free(line);
    }
}