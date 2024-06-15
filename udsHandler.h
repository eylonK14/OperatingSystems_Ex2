#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int streamServer(char *socketPath);
int streamClient(char *socketPath);
int datagramServer(char *socketPath);
int datagramClient(char *socketPath);