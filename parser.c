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

    char **command = (char **)malloc(sizeof(char *) * (amountOfCommands + 1));
    char *token = strtok(op, " ");
    int i = 0;
    while (token != NULL)
    {
        command[i] = (char *)malloc(sizeof(char) * strlen(token));
        command[i++] = token;
        token = strtok(NULL, " ");
    }
    command[amountOfCommands] = NULL;
    parser._commandParser._command = command;
    parser._hasCommand = 1;
    return parser;
}

int createNetworkSocket(char *ip, int *port, char *networkData, int (*serverFunction)(int), int (*clientFunction)(int, char *))
{
    int sockfd = -1;
    char serverOrClient = networkData[3];
    if (serverOrClient == 'S')
    {
        *port = atoi(networkData + 4);
        int p = *port;
        sockfd = serverFunction(p);
    }
    else if (serverOrClient == 'C')
    {
        ip = networkData + 4;
        *port = atoi(strchr(ip, ':') + 1);
        int p = *port;
        ip[strlen(ip) - strlen(strchr(ip, ':'))] = '\0';
        sockfd = clientFunction(p, ip);
    }
    else
        exit(EXIT_FAILURE);

    return sockfd;
}

int unixDomainSocketParser(char *networkData)
{
    int sockfd = -1;
    char serverOrClient = networkData[3];
    char tcpOrUdp = networkData[4];
    char *path = networkData + 5;
    if (serverOrClient == 'S')
    {
        if (tcpOrUdp == 'S')
            sockfd = streamServer(path);
        else if (tcpOrUdp == 'D')
            sockfd = datagramServer(path);
        else
            exit(EXIT_FAILURE);
    }
    else if (serverOrClient == 'C')
    {
        if (tcpOrUdp == 'S')
            sockfd = streamClient(path);
        else if (tcpOrUdp == 'D')
            sockfd = datagramClient(path);
        else
            exit(EXIT_FAILURE);
    }
    else
        exit(EXIT_FAILURE);
    return sockfd;
}

networkParser parseNetworkData(networkParser netParse, char opt, char *networkData)
{

    int sockfd = -1, port = 0;
    char type[3];
    char *ip = NULL;
    int hasConnectionType = 0;
    strncpy(type, networkData, 3);
    if (strcmp(type, "TCP") == 0)
    {
        sockfd = createNetworkSocket(ip, &port, networkData, tcpServer, tcpClient);
        hasConnectionType = 1;
        printf("TCP sockfd: %d\n", sockfd);
    }
    else if (strcmp(type, "UDP") == 0)
    {
        sockfd = createNetworkSocket(ip, &port, networkData, udpServer, udpClient);
        hasConnectionType = 1;
        printf("UDP sockfd: %d\n", sockfd);
    }
    else if (strcmp(type, "UDS") == 0)
    {
        sockfd = unixDomainSocketParser(networkData);
        hasConnectionType = 1;
    }
    else
        exit(EXIT_FAILURE);

    if (opt == 'i')
    {
        netParse._inSockfd = sockfd;
        netParse._i++;
    }
    else if (opt == 'o')
    {
        netParse._outSockfd = sockfd;
        netParse._o++;
    }
    else if (opt == 'b')
    {
        netParse._inSockfd = sockfd;
        netParse._outSockfd = sockfd;
    }
    netParse._port = port;
    netParse._ip = ip;
    netParse._connectionType = NULL;
    netParse._hasConnectionType = hasConnectionType;
    netParse._connectionType = (char *)malloc(sizeof(char) * 3);
    strncpy(netParse._connectionType, type, 3);
    return netParse;
}

networkParser parseArgs(int argc, char **argv)
{
    networkParser netParse;
    netParse._inSockfd = STDIN_FILENO;
    netParse._outSockfd = STDOUT_FILENO;
    netParse._hasCommand = 0;
    netParse._hasConnectionType = 0;
    netParse._i = 0;
    netParse._o = 0;

    int opt = 0;
    while ((opt = getopt(argc, argv, "e:i:o:b:t:")) != -1)
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
        case 't':
            netParse._timeout = atoi(optarg);
            break;
        default:
            netParse._commandParser._successCode = 0;
            return netParse;
        }
    }
    netParse._commandParser._successCode = 1;

    return netParse;
}