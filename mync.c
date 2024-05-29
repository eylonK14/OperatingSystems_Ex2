#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PROGRAM 2

void execute_command(char **cmd, int input_fd, int output_fd)
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
    commandParser parseCommand;

    parseCommand = parseArgs(argc, argv);

    if (!parseCommand._successCode)
    {
        printf("Usage: %s -e \"<command> <args>\" [-i|-o|-b] [TCPS<PORT> | TCPC<ip:port> | TCPC<hostname:port>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    execute_command(parseCommand._command, STDIN_FILENO, STDOUT_FILENO);

    return 0;
}