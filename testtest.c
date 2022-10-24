#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *read_line()
{
    char *line = NULL;
    size_t bufferlength = 0;
    getline(&line, &bufferlength, stdin);
    return line;
}
void kash_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "kash: cd: missing argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("kash: cd");
        }
    }
}
char **split_line(char *line)
{
    int len = 0;
    int cap = 16;
    char **split_lines = malloc(cap * sizeof(char *));

    char *del = " \t\r\n";
    char *token = strtok(line, del);

    while (token != NULL)
    {
        split_lines[len] = token;
        length++;

        if (len >= cap)
        {
            cap = (int)(cap * 1.5);
            split_lines = realloc(split_lines, cap * sizeof(char *));
        }

        token = strtok(NULL, del);
    }

    split_lines[length] = NULL;
    return split_lines;
}

void execute(char **args)
{
    if(args[0]=='cd'){
        kash_cd(args);
    }
    pid_t child = fork();
    if (child == 0)
    {
        execvp(args[0], args);
        perror("Error: ");
        exit(1);
    }
    else if (child > 0)
    {
        int status;
        do
        {
            waitpid(child, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    else
    {
        perror("Error: ");
    }
}
int main()
{
    while (true)
    {
        printf("This Shell is created by Mahir\n\n")
            printf("$ ");
        char *line = read_line();
        char **split_lines = split_line(line);

        if (split_lines[0] != NULL)
        {
            execute(split_lines);
        }

        free(split_lines);
        free(line);
    }
}
