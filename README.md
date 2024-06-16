# mync - a netcat clone

This project implements a Tic-Tac-Toe game, and uses a netcat clone called mync in order to execute it over processes.

note: every stage has the capabilities of all the previous stages.

## Table of Contents

- [mync - a netcat clone](#mync---a-netcat-clone)
  - [Table of Contents](#table-of-contents)
  - [Ex1 - Basic Tic-Tac-Toe game](#ex1---basic-tic-tac-toe-game)
  - [Ex2 - Basic mync](#ex2---basic-mync)
  - [Ex3 - Implementing TCP based connection](#ex3---implementing-tcp-based-connection)
  - [Ex3.5 - Implementing chat capability](#ex35---implementing-chat-capability)
  - [Ex4 - Adding UDP support](#ex4---adding-udp-support)
  - [Ex5 - TCPMUX](#ex5---tcpmux)
  - [Ex6 - Adding UDS  (Unix Domain Sockets) support](#ex6---adding-uds--unix-domain-sockets-support)
  - [How to use](#how-to-use)
  - [Function list and description](#function-list-and-description)
    - [ttt.c](#tttc)
    - [mync.c](#myncc)
    - [parser.c](#parserc)
    - [tcpHandler.c](#tcphandlerc)
    - [udpHandler.c](#udphandlerc)
    - [udsHandler.c](#udshandlerc)
  - [Authors](#authors)
  - [References](#references)

## Ex1 - Basic Tic-Tac-Toe game

In this stage, we've only implemented the Tic-Tac-Toe game.
The game logic is given to the program as a command line argument.

## Ex2 - Basic mync

In this stage, mync is only able to execute a given command locally, assuming the -e flag is given.

## Ex3 - Implementing TCP based connection

In this stage, mync is given the option to use TCP services (servers and clients) as his I\O source. using the -i and -o flags we can redirect the streams.

## Ex3.5 - Implementing chat capability

In this stage, if the program mync doesnt get the -e flag, than it will behave as an echoing chat.

## Ex4 - Adding UDP support

In this stage, mync is given the option to use UDP services (servers and clients) as his I\O source. using the -i and -o flags we can redirect the streams. In order to close a UDP connection, we will have to use the -t flag, giving an amount of seconds for timeout. We can also mix between TCP and UDP connection.

## Ex5 - TCPMUX

not implemented.

## Ex6 - Adding UDS  (Unix Domain Sockets) support

In this stage, mync is given the option to use UDS services (both DATAGRAM/STREAM servers and clients) as his I\O source. using the -i and -o flags we can redirect the streams. We can also mix between TCP, UDP and UDS connection.

## How to use

1. Stage 1:

   ```c
   ./ttt 123456789
   ```

2. Stage 2:

   ```c
   ./mync -e "./ttt 123456789"
   ```

3. Stage 3 and 3.5:

   ```c
   //Stage 3:

   ./mync -e "./ttt 123456789" -i TCPS4050
   ./mync -e "./ttt 123456789" -b TCPS4050 
   ./mync -e "./ttt 123456789" -i TCPS4050 -o TCPClocalhost:4455

   //Stage 3.5: 
   
   ./mync
   ./mync -i TCPS4050
   ./mync -b TCPS4050 
   ./mync -i TCPS4050 -o TCPClocalhost:4455
   ```

4. Stage 4:

   ```c
   ./mync -e "./ttt 123456789" -i UDPS4050 -t 10
   ./mync -e "./ttt 123456789" -i UDPS4050 -o TCPClocalhost:4455
   ```

5. Stage 5:

   not implemented.

6. Stage 6:

   ```c
   ./mync -e "./ttt 123456789" -i UDSSS/tmp/socket
   ./mync -e "./ttt 123456789" -i UDSCD/tmp/socket
   ./mync -e "./ttt 123456789" -i UDSSD/tmp/socket
   ./mync -e "./ttt 123456789" -i UDSCS/tmp/socket
   ```

## Function list and description

### ttt.c

```c
void printFlushed(char *s)
int valid(int argc, char **argv)
void print(int board[BOARD_SIZE][BOARD_SIZE])
int putInLocation(int board[BOARD_SIZE][BOARD_SIZE], int i, int j, int sign)
int makeAplay(int board[BOARD_SIZE][BOARD_SIZE], int move, int sign)
int over(int board[BOARD_SIZE][BOARD_SIZE])
int move(char *moveList, int board[BOARD_SIZE][BOARD_SIZE], int *turn)
int main(int argc, char **argv)
```

the main function is the driver code of the program.

the move function is used to make a move in the game.

the over function is used to check if the game is over.

the makeAplay function is used to make a play in the game.

the putInLocation function is used to put a sign in a location.

the print function is used to print the game board.

the valid function is used to check if the game is valid.

the printFlushed function is used to print a string without a newline.

### mync.c

```c
void handle_alarm(int sig)
void handle_client(int sockfd, int inOrOut)
void execute_command(networkParser parseCommand)
int main(int argc, char *argv[])
```

the main function is the driver code of the program.

the handle_alarm function is used to handle the alarm signal.

the handle_client function is used to handle the case where the program didn't receive the -e flag.

the execute_command function is used to execute the command given to the program.

### parser.c

```c
networkParser parseCommand(networkParser parser, char *op)
int createNetworkSocket(char *ip, int *port, char *networkData, int (*serverFunction)(int), int (*clientFunction)(int, char *))
int unixDomainSocketParser(char *networkData)
networkParser parseNetworkData(networkParser netParse, char opt, char *networkData)
networkParser parseArgs(int argc, char **argv)
```

the parseArgs function is used to parse the arguments given to the program.

the parseNetworkData function is used to parse the network data.

the unixDomainSocketParser function is used to parse the UDS data.

the createNetworkSocket function is is used to create the network socket.

the parseCommand function is used to parse the command given to the program.

### tcpHandler.c

```c
int tcpServer(int port)
int tcpClient(int port, char *ip)
```

the tcpServer function is used to create a TCP server.

the tcpClient function is used to create a TCP client.

### udpHandler.c

```c
int udpServer(int port)
int udpClient(int port, char *ip)
```

the udpServer function is used to create a UDP server.

the udpClient function is used to create a UDP client.

### udsHandler.c

```c
int streamServer(char *socketPath)
int streamClient(char *socketPath)
int datagramServer(char *socketPath)
int datagramClient(char *socketPath)
```

the streamServer function is used to create a stream server.

the streamClient function is used to create a stream client.

the datagramServer function is used to create a datagram server.

the datagramClient function is used to create a datagram client.

## Authors

- [Noam Leshem](https://github.com/noamleshem)
- [Eylon Yaakov Katan](https://github.com/eylonk14)

## References

- [Tic-Tac-Toe game](https://en.wikipedia.org/wiki/Tic-tac-toe)
- [Netcat](https://en.wikipedia.org/wiki/Netcat)
- [Unix Domain Sockets](https://en.wikipedia.org/wiki/Unix_domain_socket)
- [TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol)
- [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/index.html)
- [Beej's Guide to Interprocess Communication](https://beej.us/guide/bgipc/html/index.html)
