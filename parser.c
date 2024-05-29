#include <getopt.h>

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
            {
                port = atoi(networkData + 4);
                o = tcpServer(port);
            }
            else if (serverOrClient == 'C')
            {
                ip = networkData + 4;
                port = atoi(strchr(ip, ':') + 1);
                ip[strlen(ip) - strlen(strchr(ip, ':'))] = '\0';
                o = tcpClient(port, ip);
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