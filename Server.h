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
extern time_t time ();		
in_port_t servPort;			// This will hold which port the server is using
int svrSock;				
int clntSock;				
int client_len;			
struct sockaddr_in server;		
struct sockaddr_in client;		
char clientAddress[BUFSIZE];		// The address of the client
ssize_t numBytes;			// Used for error handling
pid_t pid;				
int status;				

//These are the functions used in this file
void createSocket();			
void constructServerAddress();	
void assignSocketToAddress();		
void findConnection(); 		
int acceptConnection();		
void newClientInfo();			
void play_hangman(int in, int out);	
void sendFinalMessage();		

#endif // SERVER_H
