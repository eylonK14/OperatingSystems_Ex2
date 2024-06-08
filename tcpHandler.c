#include "tcpHandler.h"

int tcpServer(int port)
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

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
    return client_sock;
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
        char *ipversion;

        // get the pointer to the address itself
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
        ipversion = "IPv4";

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipversion, ipstr);
    }

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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