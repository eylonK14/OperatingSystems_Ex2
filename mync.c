#include "parser.h"
#include <signal.h>

pid_t id = 0;

void handle_alarm(int sig)
{
    printf("Process No. %d timed out\n", sig);
    if (id)
        kill(id, SIGKILL);
}

void handle_client(int sockfd, int inOrOut)
{
    char buffer[BUFFER_SIZE];
    fd_set fds;
    int n;

    while (1)
    {
        FD_ZERO(&fds);
        if (inOrOut)
        {
            FD_SET(sockfd, &fds);
            FD_SET(STDIN_FILENO, &fds);
        }
        else
        {
            FD_SET(sockfd, &fds);
        }

        int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;
        select(max_fd + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &fds))
        {
            if ((n = read(sockfd, buffer, sizeof(buffer) - 1)) <= 0)
                break;
            buffer[n] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &fds))
        {
            if ((n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) <= 0)
                continue;
            buffer[n] = '\0';
            write(sockfd, buffer, n);
        }
    }

    close(sockfd);
}

void execute_command(networkParser parseCommand)
{
    int input_fd = parseCommand._inSockfd;
    int output_fd = parseCommand._outSockfd;
    char **cmd = parseCommand._commandParser._command;

    if (parseCommand._connectionType != NULL && strcmp(parseCommand._connectionType, "UDP") == 0)
    {
        alarm(parseCommand._timeout);
    }

    if (!parseCommand._hasCommand)
    {
        if (input_fd != STDIN_FILENO)
            handle_client(input_fd, 1);
        else if (output_fd != STDOUT_FILENO)
            handle_client(output_fd, 0);
        else if    (input_fd == STDIN_FILENO && output_fd == STDOUT_FILENO)
            handle_client(input_fd, 1);

        return;
    }

    if ((id = fork()) == 0)
    {
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        execvp(cmd[0], cmd);
        kill(id, SIGKILL);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(output_fd);
        close(input_fd);
        wait(NULL);
        alarm(0);
    }
}

int main(int argc, char **argv)
{
    networkParser parseCommand;

    parseCommand = parseArgs(argc, argv);

    if (!parseCommand._commandParser._successCode)
    {
        printf("Usage: %s [-e \"<command> <args>\"] [-i|-o|-b] [TCPS<PORT> | TCPC<ip:port> | TCPC<hostname:port>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGALRM, handle_alarm);

    execute_command(parseCommand);

    return 0;
}