#include "parser.h"

void execute_command(networkParser parseCommand)
{

    int input_fd = parseCommand._inSockfd;
    int output_fd = parseCommand._outSockfd;
    char **cmd = parseCommand._commandParser._command;
    pid_t id;
    if ((id = fork()) == 0)
    {
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        if (parseCommand._execute)
        {
            execvp(cmd[0], cmd);
            kill(id, SIGKILL);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        // if command is not provided, make the two file descriptors talk to each other
        else
        {
            char buffer[1024];
            int n;
            printf("the input fd is: %d\n", input_fd);
            printf("the output fd is: %d\n", output_fd);
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(STDIN_FILENO, &read_fds);
            FD_SET(input_fd, &read_fds);
            int max_fd = MAX(input_fd, STDIN_FILENO) + 1;
            int activity = select(max_fd, &read_fds, NULL, NULL, NULL);

            if (activity < 0 && errno != EINTR)
            {
                perror("Error in select");
                exit(EXIT_FAILURE);
            }

            if (FD_ISSET(STDIN_FILENO, &read_fds))
            {
                printf("the stdin is set\n");
                while ((n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
                {
                    buffer[n] = '\0';
                    // printf("the buffer is: %s\n", buffer);
                    write(input_fd, buffer, n);
                }
            }
            else if (FD_ISSET(input_fd, &read_fds))
            {
                printf("the input fd is set\n");
                while ((n = read(input_fd, buffer, sizeof(buffer) - 1)) > 0)
                {
                    buffer[n] = '\0';
                    // printf("the buffer is: %s\n", buffer);
                    write(STDOUT_FILENO, buffer, n);
                }
            }
            else
            {
                printf("the file descriptors are not set\n");
            }
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        close(output_fd);
        close(input_fd);
        wait(NULL);
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

    execute_command(parseCommand);

    return 0;
}