#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PROGRAM 2

typedef struct _commandParser
{
    char **_command;
    int _successCode;
} commandParser;

typedef struct _networkParser
{
    struct _commandParser _commandParser;
    int _inSockfd;
    int _outSockfd;
    int _port;
    char *_ip;
} networkParser;

int tcpServer(int port)
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    double total_t;

    int opt = 1;
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind(2)");
        close(sock);
        return 1;
    }

    if (listen(sock, 1) < 0)
    {
        perror("listen(2)");
        close(sock);
        return 1;
    }

    printf("Waiting for TCP connection...\n");
    int client_sock = accept(sock, (struct sockaddr *)&client, &client_len); // try to connect
    if (client_sock < 0)
    {
        perror("accept(2)");
        close(sock);
        return 1;
    }
}

int tcpClient(int port, char *clientData)
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(clientData, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", clientData);

    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;

        // get the pointer to the address itself
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("Starting Sender.\n");
    if (sock < 0)
    {
        perror("socket(2)");
        return 1;
    }

    if (inet_pton(AF_INET, ipstr, &server.sin_addr) <= 0)
    {
        perror("inet_pton(3)");
        close(sock);
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    printf("Connecting to Reciever...\n");
    int con = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (con < 0)
    {
        perror("connect(2)");
        close(sock);
        return 1;
    }

    freeaddrinfo(res); // free the linked list

    return sock;
}

int words(const char sentence[])
{
    int counted = 0;

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
    return command;
}

commandParser parseArgs(int argc, char *argv[])
{
    commandParser comParse;
    if (argc < PROGRAM)
    {
        comParse._successCode = 0;
        return comParse;
    }

    int i = -1;
    int o = -1;
    int opt;
    int port;
    char *ip;
    char *networkData;
    char serverOrClient;
    char type[3];
    char **command;

    while ((opt = getopt(argc, argv, "e:i:o:b")) != -1)
    {
        switch (opt)
        {
        case 'e':
            command = parseCommand(optarg);
            break;
        case 'b':
            o = STDERR_FILENO;
        case 'i':
            i = STDIN_FILENO;
        case 'o':
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
                comParse._successCode = 0;
                return comParse;
            }

            if (i == 0)
            {
                o = STDOUT_FILENO;
            }
            break;
        default:
            comParse._successCode = 0;
            return comParse;
        }
    }
    printf("network data: %s\n", networkData);
    printf("type: %s\n", type);
    printf("server or client: %c\n", serverOrClient);
    printf("ip: %s\n", ip);
    printf("port: %d\n", port);

    // build the network parser:
    networkParser netParse;
    netParse._commandParser._command = command;
    netParse._commandParser._successCode = 1;
    netParse._port = port;
    netParse._ip = ip;
    netParse._inSockfd = i;
    netParse._outSockfd = o;

    comParse._successCode = 1;
    comParse._command = command;
    tcpClient(port, ip);
    return comParse;
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