 /* Header file for network client */
 /* File: Client.h */
 
#ifndef CLIENT_H_
#define CLIENT_H_

// Generic header files
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Networking header files
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Project header files
#include "Assignment.h"

# define LINESIZE 80

// These are the variables used in this file
struct sockaddr_in server;	// server's address is assembled here
struct hostent * host_info;	// host's info is stored here
int sock;			// socket descriptor for client
int count;			// simple count variable used for loop
char i_line[LINESIZE];		// description
char o_line[LINESIZE];		// description
char * server_name;		// name of server
in_port_t servPort;		// descriptionn

// These are the functions used in this file
void createSocket();
void getHostInfo(char * name);
void constructServerAddress();
void establishConnection();
void doStuff();

#endif // CLIENT_H_
