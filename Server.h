 /* Header files for network server */
 /* File: Server.c */

#ifndef SERVER_H_
#define SERVER_H_

// Generic header files
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Networking header files
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // used for inet_ntop
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

// Project header files
#include "Assignment.h"

//These are the variables used in this file
extern time_t time ();			// description
in_port_t servPort;			// This will hold which port the server is using
int svrSock;				// description
int clntSock;				// description
int client_len;			// description
struct sockaddr_in server;		// description
struct sockaddr_in client;		// description
char clientAddress[BUFSIZE];		// The address of the client
ssize_t numBytes;			// Used for error handling
pid_t pid;				// description
int status;				// description

//These are the functions used in this file
void createSocket();			// description
void constructServerAddress();	// description
void assignSocketToAddress();		// description
void findConnection(); 		// description
int acceptConnection();		// description
void newClientInfo();			// description
void play_hangman(int in, int out);	// description
void sendFinalMessage();		// description

#endif // SERVER_H
