#include "udsHandler.h"

int streamServer(char *socketPath)
{
    int s, s2, len;
    struct sockaddr_un remote, local = {.sun_family = AF_UNIX,};

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    strcpy(local.sun_path, socketPath);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    socklen_t slen = sizeof(remote);
    if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1)
    {
        perror("accept");
        exit(1);
    }

    return s2;
}

int streamClient(char *socketPath)
{
    int s, len;
    struct sockaddr_un remote = {.sun_family = AF_UNIX,};

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    strcpy(remote.sun_path, socketPath);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1)
    {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");

    return s;
}

int datagramServer(char *socketPath)
{
    int s, len;
    struct sockaddr_un local = {.sun_family = AF_UNIX};

    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    strcpy(local.sun_path, socketPath);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    return s;
}

int datagramClient(char *socketPath)
{
    int sockfd;
    struct sockaddr_un addr;

    // Create a socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        return -1;
    }

    // Configure the socket address structure
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);

    // Connect to the server socket
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        close(sockfd);
        return -1;
    }

    // Return the socket file descriptor
    return sockfd;
}
