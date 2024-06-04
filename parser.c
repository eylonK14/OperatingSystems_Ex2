#include "parser.h"

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

int parseNetworkData(char *networkData, int *port, char *ip)
{
    int sockfd = -1;
    char type[3];
    strncpy(type, networkData, 3);
    char serverOrClient = networkData[3];
    if (serverOrClient == 'S')
    {
        port = atoi(networkData + 4);
        sockfd = tcpServer(port);
    }
    else if (serverOrClient == 'C')
    {
        ip = networkData + 4;
        port = atoi(strchr(ip, ':') + 1);
        ip[strlen(ip) - strlen(strchr(ip, ':'))] = '\0';
        sockfd = tcpClient(port, ip);
    }
    return sockfd;
}

networkParser parseArgs(int argc, char *argv[])
{
    networkParser netParse;
    if (argc < PROGRAM)
    {
        netParse._commandParser._successCode = 0;
        return netParse;
    }

    int i = STDIN_FILENO;
    int o = STDOUT_FILENO;
    int opt;
    int port;
    char *ip;
    char **command;

    while ((opt = getopt(argc, argv, "e:i:o:b")) != -1)
    {
        switch (opt)
        {
        case 'e':
            command = parseCommand(optarg);
            break;
        case 'b':
            i = parseNetworkData(optarg, &port, &ip);
            o = parseNetworkData(optarg, &port, &ip);
            break;
        case 'i':
            i = parseNetworkData(optarg, &port, &ip);
            break;
        case 'o':
            o = parseNetworkData(optarg, &port, &ip);
            break;
        default:
            netParse._commandParser._successCode = 0;
            return netParse;
        }
    }
    // printf("network data: %s\n", networkData);
    //printf("type: %s\n", type);
    // printf("server or client: %c\n", serverOrClient);
    printf("ip: %s\n", ip);
    printf("port: %d\n", port);

    // build the network parser:
    netParse._commandParser._command = command;
    netParse._commandParser._successCode = 1;
    netParse._port = port;
    netParse._ip = ip;
    netParse._inSockfd = i;
    netParse._outSockfd = o;

    // tcpClient(port, ip);
    return netParse;
}