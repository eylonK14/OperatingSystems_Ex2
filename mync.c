#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#include "parser.h"

void execute_command(char **cmd, int input_fd, int output_fd)
{
    pid_t id;
    if ((id = fork()) == 0)
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
        kill(id, SIGKILL);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    networkParser parseCommand;

    parseCommand = parseArgs(argc, argv);

    if (!parseCommand._commandParser._successCode)
    {
        printf("Usage: %s -e \"<command> <args>\" [-i|-o|-b] [TCPS<PORT> | TCPC<ip:port> | TCPC<hostname:port>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    execute_command(parseCommand._commandParser._command, parseCommand._inSockfd, parseCommand._outSockfd);

    return 0;
}