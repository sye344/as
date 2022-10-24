

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char cur_path[256];
void change_dir(char *pth) {
	int ret;
	if (pth[0] == '.') {
		getcwd(cur_path, sizeof(cur_path));
		strcat(cur_path,"/");
		strcat(cur_path,pth);
		ret = chdir(cur_path);	
	}
	else if (pth[0] == '~') {
		pth++;
		char *cur_path = getenv("HOME");
		strcat(cur_path, pth);
		ret = chdir(cur_path);
	}
	else {
		ret = chdir(pth);
	}
	if (ret == -1) {
		printf("Invalid Path\n");
	}
	else {
		char present[10001];
		getcwd(present, sizeof(present));
		printf("%s\n", present);
	}
}

char *read_line()
{
    char *line = NULL;
    size_t bufferlength = 0;
    getline(&line, &bufferlength, stdin);
    return line;
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
        len++;

        if (len >= cap)
        {
            cap = (int)(cap * 1.5);
            split_lines = realloc(split_lines, cap * sizeof(char *));
        }

        token = strtok(NULL, del);
    }

    split_lines[len] = NULL;
    return split_lines;
}

void execute(char **args)
{
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
    if(strcmp(args[0],"cd")==0){
        change_dir(args[1]);
        continue;
    }
    while (true)
    {
        printf("This Shell is created by Mahir\n\n");
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
