#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <errno.h>

#define PROGRAM 2

struct network
{
    char** command;
    int successCode;
    int inSockfd;
    int outSockfd;
    int port;
    char* ip;
};

int tcp_server()
{
    return 0;
}

int tcp_client()
{
    return 0;
}

int words(const char sentence[])
{
    int counted = 0; // result

    // state:
    const char *it = sentence;
    int inword = 0;

    do
        switch (*it)
        {
        case '\0':
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            if (inword)
            {
                inword = 0;
                counted++;
            }
            break;
        default:
            inword = 1;
        }
    while (*it++);

    return counted;
}

char **parseCommand(char *op)
{
    int amountOfCommands = words(op);

    char **command = malloc(sizeof(char *) * (amountOfCommands + 1));
    char *token = strtok(op, " ");
    int i = 0;
    while (token != NULL)
    {
        command[i] = malloc(sizeof(char) * strlen(token));
        command[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    command[amountOfCommands] = NULL;
    for (int i = 0; i <= amountOfCommands; i++)
    {
        printf("command[%d]: %s\n", i, command[i]);
    }
    return command;
}

int parse_args(int argc, char *argv[], char **command, int port, char *ip)
{
    if (argc < PROGRAM)
    {
        // printf("1 - argc < PROGRAM\n");
        return -1;
    }

    int i = -1;
    int o = -1;
    int opt;
    char *networkData;
    char serverOrClient;
    char type[3];

    while ((opt = getopt(argc, argv, "e:i:o:b")) != -1)
    {
        printf("opt = %d\n", opt);
        switch (opt)
        {
        case 'e':
            // PARSE ARGS
            printf("command: %s\n", optarg);
            command = parseCommand(optarg);
            break;
        case 'b':
            o = STDERR_FILENO;
        case 'i':
            i = STDIN_FILENO;
        case 'o':
            printf("optarg: %s\n", optarg);
            networkData = optarg;
            strncpy(type, networkData, 3);
            serverOrClient = networkData[3];
            if (serverOrClient == 'S')
                port = atoi(networkData + 4);
            else if (serverOrClient == 'C')
            {
                ip = networkData + 4;
                port = atoi(strchr(ip, ':') + 1);
                ip[strlen(ip) - strlen(strchr(ip, ':'))] = '\0';
            }
            else
            {
                printf("2 - serverOrClient\n");
                return 0;
            }
            
            if (i == 0)
            {
                o = STDOUT_FILENO;
            }
            printf("type: %s\n", type);
            break;
        default:
            printf("3 - default\n");
            return 0;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        printf("command[%d]: %s\n", i, command[i]);
    }
    return 1;
}

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
    char **command;
    int port;
    char *ip;

    int a = parse_args(argc, argv, command, port, ip);
    printf("%d", a);

    if (!a)
    {
        printf("Usage: %s -e \"<command> <args>\" [-i|-o|-b] [TCPS<PORT> | TCPC<ip:port> | TCPC<hostname:port>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++)
    {
        printf("command[%d]: %s\n", i, command[i]);
    }

    execute_command(command, STDIN_FILENO, STDOUT_FILENO);

    return 0;
}