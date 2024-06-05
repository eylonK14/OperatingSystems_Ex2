#pragma once

#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "tcpHandler.h"
#include "udpHandler.h"

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

networkParser parseArgs(int, char **);
