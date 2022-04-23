 /* Network client for hangman game */
 /* File: Client.c */

// Project header files
#include "Client.h"
 	
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
	// Create a socket that supports dual stack
	sock = socket (AF_UNSPEC, SOCK_STREAM, 0);
	
	if (sock <0) 
	{
		perror ("Creating stream socket");
 		exit (1);
	}
}

void getHostInfo(char * name)
{
	/* Find information about the host*/
	host_info = gethostbyname(server_name);
	
	// Check if information about the host cannot be found
 	if (host_info == NULL) 
 	{
 		fprintf (stderr, "%s: unknown host:%s \n", name, server_name);
 		exit (2);
 	}
}

void constructServerAddress()
{
	// Contruct the address of the found connection
	server.sin_family = host_info->h_addrtype;
 	memcpy ((char *) & server.sin_addr, host_info->h_addr, host_info->h_length);
 	server.sin_port = htons (servPort);
}

void establishConnection()
{
	// Connect socket to server socket
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
