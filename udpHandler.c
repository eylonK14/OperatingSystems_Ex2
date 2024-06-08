
#include "udpHandler.h"

// // get sockaddr, IPv4 or IPv6:
// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET)
//         return &(((struct sockaddr_in *)sa)->sin_addr);

//     return &(((struct sockaddr_in6 *)sa)->sin6_addr);
// }

// int udpServer(int port)
// {
//     int sockfd;
//     struct addrinfo hints, *serverinfo, *p;
//     int rv;
//     // int numbytes;
//     // struct sockaddr_storage their_addr;
//     // char buf[MAX_BUFFER_LEN];
//     // socklen_t addr_len;
//     // char s[INET_ADDRSTRLEN];

//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET; // set to AF_INET to use IPv4
//     hints.ai_socktype = SOCK_DGRAM;
//     hints.ai_flags = AI_PASSIVE; // use my IP

//     char *portStr = malloc(6);
//     snprintf(portStr, 6, "%d", port);

//     if ((rv = getaddrinfo(NULL, portStr, &hints, &serverinfo)) != 0)
//     {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//         return -1;
//     }

//     // loop through all the results and bind to the first we can
//     for (p = serverinfo; p != NULL; p = p->ai_next)
//     {
//         if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1)
//         {
//             perror("listener: socket");
//             continue;
//         }

//         if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
//         {
//             close(sockfd);
//             perror("listener: bind");
//             continue;
//         }

//         break;
//     }

//     if (p == NULL)
//     {
//         fprintf(stderr, "listener: failed to bind socket\n");
//         return -2;
//     }

//     freeaddrinfo(serverinfo);

//     // printf("listener: waiting to recvfrom...\n");

//     // addr_len = sizeof their_addr;
//     // if ((numbytes = recvfrom(sockfd, buf, MAX_BUFFER_LEN-1 , 0,
//     //     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
//     //     perror("recvfrom");
//     //     exit(1);
//     // }

//     // printf("listener: got packet from %s\n",
//     //     inet_ntop(their_addr.ss_family,
//     //         get_in_addr((struct sockaddr *)&their_addr),
//     //         s, sizeof s));
//     // printf("listener: packet is %d bytes long\n", numbytes);
//     // buf[numbytes] = '\0';
//     // printf("listener: packet contains \"%s\"\n", buf);

//     // close(sockfd);

//     return sockfd;
// }

// int udpClient(int port, char *clientData)
// {
//     int sockfd;
//     struct addrinfo hints, *serverinfo, *p;
//     int rv;
//     // int numbytes;

//     // if (argc != 3) {
//     //     fprintf(stderr,"usage: talker hostname message\n");
//     //     exit(1);
//     // }

//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET; // set to AF_INET to use IPv4
//     hints.ai_socktype = SOCK_DGRAM;

//     char *portStr = malloc(6);
//     snprintf(portStr, 6, "%d", port);

//     if ((rv = getaddrinfo(clientData, portStr, &hints, &serverinfo)) != 0)
//     {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//         return -1;
//     }

//     // loop through all the results and make a socket
//     for (p = serverinfo; p != NULL; p = p->ai_next)
//     {
//         if ((sockfd = socket(p->ai_family, p->ai_socktype,
//                              p->ai_protocol)) == -1)
//         {
//             perror("talker: socket");
//             continue;
//         }

//         break;
//     }

//     if (p == NULL)
//     {
//         fprintf(stderr, "talker: failed to create socket\n");
//         return -2;
//     }

//     // if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
//     //          p->ai_addr, p->ai_addrlen)) == -1) {
//     //     perror("talker: sendto");
//     //     exit(1);
//     // }

//     // freeaddrinfo(serverinfo);

//     // printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
//     // close(sockfd);

//     return sockfd;
// }

/*** IBMCOPYR ********************************************************/
/*                                                                   */
/* Component Name: UDPS                                              */
/*                                                                   */
/*                                                                   */
/* Copyright:    Licensed Materials - Property of IBM                */
/*                                                                   */
/*               "Restricted Materials of IBM"                       */
/*                                                                   */
/*               5647-A01                                            */
/*                                                                   */
/*               (C) Copyright IBM Corp. 1977, 1998                  */
/*                                                                   */
/*               US Government Users Restricted Rights -             */
/*               Use, duplication or disclosure restricted by        */
/*               GSA ADP Schedule Contract with IBM Corp.            */
/*                                                                   */
/* Status:       CSV2R6                                              */
/*                                                                   */
/*  SMP/E Distribution Name: EZAEC021                                */
/*                                                                   */
/*** IBMCOPYR ********************************************************/

// static char ibmcopyr[] =
//     "UDPS     - Licensed Materials - Property of IBM. "
//     "This module is \"Restricted Materials of IBM\" "
//     "5647-A01 (C) Copyright IBM Corp. 1992, 1996. "
//     "See IBM Copyright Instructions.";

// #include <sys/socket.h>
// #include <netdb.h>
// #include <stdio.h>

main()
{
    int s, namelen, client_address_size;
    struct sockaddr_in client, server;
    char buf[32];

    /*
     * Create a datagram socket in the internet domain and use the
     * default protocol (UDP).
     */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        tcperror("socket()");
        exit(1);
    }

    /*
     * Bind my name to this socket so that clients on the network can
     * send me messages. (This allows the operating system to demultiplex
     * messages and get them to the correct server)
     *
     * Set up the server name. The internet address is specified as the
     * wildcard INADDR_ANY so that the server can get messages from any
     * of the physical internet connections on this host. (Otherwise we
     * would limit the server to messages from only one network
     * interface.)
     */
    server.sin_family = AF_INET;         /* Server is in Internet Domain */
    server.sin_port = 0;                 /* Use any available port      */
    server.sin_addr.s_addr = INADDR_ANY; /* Server's Internet Address   */

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        tcperror("bind()");
        exit(2);
    }

    /* Find out what port was really assigned and print it */
    namelen = sizeof(server);
    if (getsockname(s, (struct sockaddr *)&server, &namelen) < 0)
    {
        tcperror("getsockname()");
        exit(3);
    }

    printf("Port assigned is %d\n", ntohs(server.sin_port));

    /*
     * Receive a message on socket s in buf  of maximum size 32
     * from a client. Because the last two paramters
     * are not null, the name of the client will be placed into the
     * client data structure and the size of the client address will
     * be placed into client_address_size.
     */
    client_address_size = sizeof(client);

    if (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&client,
                 &client_address_size) < 0)
    {
        tcperror("recvfrom()");
        exit(4);
    }
    /*
     * Print the message and the name of the client.
     * The domain should be the internet domain (AF_INET).
     * The port is received in network byte order, so we translate it to
     * host byte order before printing it.
     * The internet address is received as 32 bits in network byte order
     * so we use a utility that converts it to a string printed in
     * dotted decimal format for readability.
     */
    printf("Received message %s from domain %s port %d internet address %s\n",
           buf,
           (client.sin_family == AF_INET ? "AF_INET" : "UNKNOWN"),
           ntohs(client.sin_port),
           inet_ntoa(client.sin_addr));

    /*
     * Deallocate the socket.
     */
    close(s);
}

// #define _XOPEN_SOURCE_EXTENDED 1
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <stdio.h>

main(int argc, char **argv)
{

    int s;
    unsigned short port;
    struct sockaddr_in server;
    char buf[32];

    /* argv[1] is internet address of server argv[2] is port of server.
     * Convert the port from ascii to integer and then from host byte
     * order to network byte order.
     */
    if (argc != 3)
    {
        printf("Usage: %s <host address> <port> \n", argv[0]);
        exit(1);
    }
    port = htons(atoi(argv[2]));

    /* Create a datagram socket in the internet domain and use the
     * default protocol (UDP).
     */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket()");
        exit(1);
    }

    /* Set up the server name */
    server.sin_family = AF_INET;                 /* Internet Domain    */
    server.sin_port = port;                      /* Server Port        */
    server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */

    strcpy(buf, "Hello");

    /* Send the message in buf to the server */
    if (sendto(s, buf, (strlen(buf) + 1), 0, &server, sizeof(server)) < 0)
    {
        perror("sendto()");
        exit(2);
    }

    /* Deallocate the socket */
    close(s);
}