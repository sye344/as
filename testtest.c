#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
char current[256];
void cd(char *path) {
	int ret;
	if (path[0] == '.') {
		getcwd(current, sizeof(current));
		strcat(current,"/");
		strcat(current,path);
		ret = chdir(current);	
	}
	else if (path[0] == '~') {
		path++;
		char *current = getenv("HOME");
		strcat(current, path);
		ret = chdir(current);
	}
	else {
		ret = chdir(path);
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
int main()
{

    printf("This Shell is created by Mahir\n");
    
    while (true)
    {
        printf("$ ");
        char *line = NULL;
        size_t bufferlength = 0;
        getline(&line, &bufferlength, stdin);
        char **split_lines = split_line(line);

        if (split_lines[0] != NULL)
        {
            if(strcmp(split_lines[0],"cd")==0){
                cd(split_lines[1]);
                continue;
            }
            pid_t child = fork();

            if (child == 0)
            {
                execvp(split_lines[0], args);
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

        free(split_lines);
        free(line);
    }
}
