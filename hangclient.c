/* Hangclient.c - Client for hangman server.  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Practical.h"

# define LINESIZE 80

// These are the functions used in this file
void createSocket();
void getHostInfo(char * name);
void constructServerAddress();
void establishConnection();
void doStuff();

// These are the variables used in this file
struct sockaddr_in server;	// server's address is assembled here
struct hostent * host_info;	// host's info is stored here
int sock;			// socket descriptor for client
int count;			// simple count variable used for loop
char i_line[LINESIZE];		// description
char o_line[LINESIZE];		// description
char * server_name;		// name of server
in_port_t servPort;		// descriptionn
 	
int main (int argc, char * argv [])
{
	if (argc < 2 || argc > 3)	// Test for correct number of arguments
	{
		printf("Parameter(s): <Server Address> [<Server Port>] \n");
 		exit (1);
 	}
 		
 	// get server name from the command line.  If none, use 'localhost'
 	server_name = argv [1];
 	servPort = atoi(argv[2]);	// Third arg (optional): server port.  7 is well-known echo port

 	createSocket();
 	getHostInfo(argv[0]);
 	constructServerAddress();
 	establishConnection();

 	/* OK connected to the server.  
	Take a line from the server and show it
	Take a line and send the user input to the server. 
 	Repeat until the server terminates the connection.*/
 	while ((count = read(sock, i_line, LINESIZE)) > 0) {
 		doStuff();
 	}
 	
 	exit(0);	//you should't get here, but close the connection just in case
}

void createSocket()
{
	sock = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sock <0) 
	{
		perror ("Creating stream socket");
 		exit (1);
	}
}

void getHostInfo(char * name)
{
	host_info = gethostbyname(server_name);
	
 	if (host_info == NULL) 
 	{
 		fprintf (stderr, "%s: unknown host:%s \n", name, server_name);
 		exit (2);
 	}
}

void constructServerAddress()
{
	server.sin_family = host_info->h_addrtype;
 	memcpy ((char *) & server.sin_addr, host_info->h_addr, host_info->h_length);
 	server.sin_port = htons (servPort);
}

void establishConnection()
{
 	if (connect (sock, (struct sockaddr *) & server, sizeof server) <0) 
	{
 		perror ("Connecting to server");
 		exit (3);
 	}
 	
 	printf ("Connected to server: %s \n", server_name);
}

void doStuff()
{
	write(1, i_line, count);
 	count = read(0, o_line, LINESIZE);	//0 = STDIN
 	write(sock, o_line, count);
}
