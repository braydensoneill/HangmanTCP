 /* Network server for hangman game */
 /* File: hangserver.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // used for inet_ntop
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "Practical.h" 	

# define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))	// description
# define MAXLEN 80						// Max string size
# define BUFSIZE 512						// Max size of buffer

//These are the functions used in this file
void createSocket();			//description
void constructServerAddress();	//description
void assignSocketToAddress();		//description
void findConnection(); 		//description
int acceptConnection();		//description
void newClientInfo();			//description
void play_hangman(int in, int out);	//description
void sendFinalMessage();		//description

//List of words used in the hangman game, read from 'words' text file
char *word [] = {
	# include "words"
};

//These are the variables used in this file
extern time_t time ();			//description
in_port_t servPort;			// This will hold which port the server is using
int maxlives = 12;			//description
int svrSock;				//description
int clntSock;				//description
int client_len;			//description
struct sockaddr_in server;		//description
struct sockaddr_in client;		//description
char clientAddress[BUFSIZE];		// The address of the client
ssize_t numBytes;			//Used for error handling
pid_t pid;				//description
int status;				//description
int userCount = 0;			//description

int main (int argc, char *argv[]) 
{
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

	// Create an infinite loop so the server can continuosly ssearch for clients
 	while(1)
 	{
 		// Assign the accepted socket's address to the 'clntSock' sockaddr variable
 		clntSock = acceptConnection();
 		
 		// Check for an inappropriate client socket value and give appropriate error message
 		if(clntSock < 0)
 			DieWithSystemMessage("Error on accept");
 			
 		// Used for dealing with new child processes
 		pid = fork();
 		
 		// Check for an inappropriate pid value and give the appropriate error message
 		if(pid < 0)
 			DieWithSystemMessage("Error on fork");
 			
 		// Indepentandlty/concurrently dealing with new clients
 		if(pid == 0) {
 			srand ((int) time ((long *) 0));	// Randomise seed for each new client
 			close(svrSock);			// Close server's socket
 			newClientInfo();			// Find information of new client
 			play_hangman(clntSock, clntSock);	// Play the hangman game, sending/reading data to/from client
 			sendFinalMessage();			// Send a final message before the connection ends
 			exit(0);				// End the connection
 		}
 		
		// Used for dealing with zombie processes
		waitpid(-1, &status, WNOHANG);
		
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

void play_hangman (int in, int out)
{
	unsigned char * whole_word, part_word [MAXLEN],
 	guess[MAXLEN], outbuf [MAXLEN];

 	int lives = maxlives;
 	int game_state = 'I';//I = Incomplete
 	int i, good_guess, word_length;
 	//unsigned char hostname[MAXLEN];

 	//gethostname (hostname, MAXLEN);
 	sprintf(outbuf, "Playing hangman on host: %d \n \n", servPort);
 	write(out, outbuf, strlen (outbuf));

 	/* Pick a word at random from the list */
 	whole_word = word[rand() % NUM_OF_WORDS];
 	word_length = strlen(whole_word);
 	syslog (LOG_USER | LOG_INFO, "server chose hangman word %s", whole_word);
 	printf("	Word chosen: %s \n\n", whole_word);

 	/* No letters are guessed Initially */
 	for (i = 0; i <word_length; i++)
 		part_word[i]='-';
 	
	part_word[i] = '\0';

 	sprintf (outbuf, "%hhn | Lives: %d \n Enter a letter: ", part_word, lives);
 	write (out, outbuf, strlen(outbuf));

	/* Get a letter from player guess */
 	while (game_state == 'I')
 	{
		while (read (in, guess, MAXLEN) <0) 
		{
 			if (errno != EINTR)
 				exit (4);
 			printf ("re-read the startin \n");
 		} /* Re-start read () if interrupted by signal */
 		
 		good_guess = 0;
 		for (i = 0; i <word_length; i++) 
 		{
 			if (guess [0] == whole_word [i]) 
 			{
 				good_guess = 1;
 				part_word [i] = whole_word [i];
 			}
 		}
 		
 		if (! good_guess) 
 			lives--;
 			
 		if (strcmp (whole_word, part_word) == 0) {
 			game_state = 'W'; /* W ==> User Won */
 			printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
 			printf("	Game Over\n");
 			printf("	State: victory\n");
 			printf("	Attempt: %s\n", part_word);
 			printf("	Answer: %s\n\n", whole_word);
 		}
 		else if (lives == 0) {
 			game_state = 'L'; /* L ==> User Lost */
 			strcpy (part_word, whole_word); /* User Show the word	 */
 			printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
 			printf("	Game Over\n");
 			printf("	State: defeat\n");
 			printf("	Attempt: %s\n", part_word);
 			printf("	Answer: %s\n\n", whole_word);
 		}
 		
 		sprintf (outbuf, "\n%hhn | Lives: %d \n Enter a letter: ", part_word, lives);
 		write (out, outbuf, strlen (outbuf));
 	}
}
