#include "parser.h"

int words(char sentence[])
{
    int count = 1;
    for (int i = 0; sentence[i] != '\0'; ++i)
        if (sentence[i] == ' ')
            count++;
    return count;
}

networkParser parseCommand(networkParser parser, char *op)
{
    int amountOfCommands = words(op);

    char **command = malloc(sizeof(char *) * (amountOfCommands + 1));
    char *token = strtok(op, " ");
    int i = 0;
    while (token != NULL)
    {
        command[i] = malloc(sizeof(char) * strlen(token));
        command[i++] = token;
        token = strtok(NULL, " ");
    }
    command[amountOfCommands] = NULL;
    parser._commandParser._command = command;

    return parser;
}

networkParser parseNetworkData(networkParser netParse, char opt, char *networkData)
{
    int sockfd = -1, port = 0;
    char type[3];
    char *ip = NULL;
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
        printf("the current ip is: %s\n", ip);
        sockfd = tcpClient(port, ip);
        printf("the current sockfd is: %d\n", sockfd);
    }
    else
        exit(EXIT_FAILURE);

    if (opt == 'i')
        netParse._inSockfd = sockfd;
    else if (opt == 'o')
        netParse._outSockfd = sockfd;
    else if (opt == 'b')
    {
        netParse._inSockfd = sockfd;
        netParse._outSockfd = sockfd;
    }
    netParse._port = port;
    netParse._ip = ip;

    return netParse;
}

networkParser parseArgs(int argc, char **argv)
{
    networkParser netParse;
    netParse._inSockfd = STDIN_FILENO;
    netParse._outSockfd = STDOUT_FILENO;
    if (argc < PROGRAM)
    {
        netParse._commandParser._successCode = 0;
        return netParse;
    }

    int opt = 0;
    while ((opt = getopt(argc, argv, "e:i:o:b:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            netParse = parseCommand(netParse, optarg);
            break;
        case 'b':
        case 'i':
        case 'o':
            netParse = parseNetworkData(netParse, opt, optarg);
            break;
        default:
            netParse._commandParser._successCode = 0;
            return netParse;
        }
    }
    // build the network parser:
    netParse._commandParser._successCode = 1;

    return netParse;
}