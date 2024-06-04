#pragma once

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define _POSIX_C_SOURCE=200112L

int tcpServer(int);
int tcpClient(int, char *);