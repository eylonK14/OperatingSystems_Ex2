#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define PROGRAM 2

void execute_command(char *cmd[], int input_fd, int output_fd)
{
    if (fork() == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
    }
    else
    {
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc < PROGRAM)
    {
        printf("Usage: %s -e <command> <args>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *command[argc - PROGRAM + 1];
    for (int i = 0; i < argc - PROGRAM; i++)
    {
        command[i] = malloc(sizeof(char) * strlen(argv[i + PROGRAM]));
        command[i] = argv[i + PROGRAM];
    }
    command[argc - PROGRAM] = NULL;

    execute_command(command, STDIN_FILENO, STDOUT_FILENO);

    return 0;
}