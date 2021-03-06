/* Network server for hangman game */
 /* File: Server.c */

// Project header files
#include "Server.h"

/*To do list
	Use AF UNSPEC
*/

int main (int argc, char *argv[]) 
{
	//service = args...and stuff
	
	// Test for correct number of arguments, and give appropriate error message
	if (argc != 2)
		DieWithUserMessage("Parameter(s)", "<Server Port>");
		
	// First arg: local port ascii to integer		
	servPort = atoi(argv[1]);
	
	// Check for an invalid port enrty and give appropriate error message
	if(servPort < 0 || servPort > 48912)
		DieWithUserMessage("Invalid Port number", "Port out of bounds");
 	
 	// Randomise the seed for the first client
	srand ((int) time ((long *) 0));

	// Set up the server's address
 	createSocket();
 	constructServerAddress();
	assignSocketToAddress();
	printf("Server created: Port %d \nAwaiting connections.. \n\n", servPort);

	// Allow the server to start looking for connections 	
 	findConnection();
 	
 	fd_set current_sockets, ready_sockets;
 	
 	FD_ZERO(&current_sockets);
 	FD_SET(clntSock, &current_sockets);

	// Create an infinite loop so the server can continuosly ssearch for clients
 	while(1)
 	{
 		ready_sockets = current_sockets;
 		
 		if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
 			DieWithSystemMessage("Select error");
 		}
 		
 		for(int i = 0; i < FD_SETSIZE; i++) {
 			if(FD_ISSET(i, &ready_sockets)) {
 				if(i == svrSock) {
 					clntSock = acceptConnection();
 					
 					if(clntSock < 0) {
 						DieWithSystemMessage("Error on accept");
 					}
 					
 					FD_SET(clntSock, &current_sockets);
 				} else {
 					srand ((int) time ((long *) 0));	// Randomise seed for each new client
		 			newClientInfo();			// Find information of new client
		 			play_hangman(i, i);	// Play the hangman game, sending/reading data to/from client
		 			sendFinalMessage();			// Send a final message before the connection ends
		 			FD_CLR(i, &current_sockets);
 				}
 			}
 		}
 	
 		// Close the client's socket
 		close(clntSock);
 		
 	}//end of while
 	
 	return 0;
}

void createSocket()
{
	// description
	svrSock = socket (AF_INET, SOCK_STREAM, 0);	// 0 or IPPROTO_TCP 

	// This error checking is the code Stevens wraps in his Socket Function etc
 	if (svrSock <0)
 		DieWithSystemMessage("Creating stream socket");
}

void constructServerAddress()
{
	// description
	server.sin_family = AF_INET;
 	server.sin_addr.s_addr = htonl(INADDR_ANY);
 	server.sin_port = htons(servPort);
}

void assignSocketToAddress()
{		
	// description
	if (bind(svrSock, (struct sockaddr *) & server, sizeof(server)) <0)
 		DieWithSystemMessage("Binding socket");
}

void findConnection()
{
	// description
	listen(svrSock, 5);
}

int acceptConnection(int sock)
{
	// description
	client_len = sizeof (client);
 	
 	//description
 	if ((sock = accept (svrSock, (struct sockaddr *) &client, &client_len)) <0)
 		DieWithSystemMessage("Accepting connection");
 	
 	return sock;
}

void newClientInfo()
{
	char findClientAddress[INET_ADDRSTRLEN]; // String to contain client address
	
	//Place this data into the clientAddress variable
	for(int i = 0; i < sizeof(findClientAddress); i++)
		clientAddress[i] = findClientAddress[i];
	
	//Conver numeric address into text string suitable for presentation
	if (inet_ntop(AF_INET, &client.sin_addr.s_addr, clientAddress, sizeof(clientAddress)) != NULL)
	{
		printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
		printf("	Handling New Client\n");
	}
	//If none is found, print an error
	else
		puts("Unable to get client address");
}

void sendFinalMessage()
{
	// Variables used for the goodbye message
	char finalMessage[100] = "\n\nConnection closed with cookie: ";	// String
	char cookie[50];						
	int randomNumber = rand() + 10000000 & 999999999;	//Random number between x and y.
	int messageLen;
			
	// String concatination for final message
	sprintf(cookie, "%d", randomNumber);	// Store the randomNumber value in the cooke variable
	strcat(finalMessage, cookie);	
	strcat(finalMessage, "\r\n\n");
	
	// Print the cookie to the server console	
	printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
	printf("	Connection Closed\n");
	printf("	Cookie: %s\n\n", cookie);
	
	// Send a final messag to the client
	messageLen = strlen(finalMessage);	// Determine input length. Strlen() finds the length of a string.
	numBytes = send(clntSock, finalMessage, strlen(finalMessage), 0);	// Send the string to the server
	
	// If no message is found, print an erorr messaage
	if (numBytes < 0)
		DieWithUserMessage("Send data failed","No message found");	
		
	// If a message is found but has the wrong length, print an error message
	else if (numBytes != messageLen)
		DieWithUserMessage("Send data failed", "Unexpected length");
}		
