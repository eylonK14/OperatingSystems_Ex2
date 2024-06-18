#pragma once

#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/un.h>
#include "tcpHandler.h"
#include "udpHandler.h"
#include "udsHandler.h"

#define BUFFER_SIZE 1024

typedef struct _commandParser
{

    char **_command;
    int _successCode;
} commandParser;

typedef struct _networkParser
{
    struct _commandParser _commandParser;
    int _hasCommand;
    int _inSockfd;
    int _outSockfd;
    int _hasConnectionType;
    int _i;
    int _o;
    int _port;
    char *_ip;
    char *_connectionType;
    int _timeout;
} networkParser;

networkParser parseArgs(int, char **);
