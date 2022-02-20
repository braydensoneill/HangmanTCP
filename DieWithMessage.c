/*
The purpose of these functions are to print error messages in the TCPEchoClient4.c file. There are two parameters: msg and detail. "Msg" prints out an error message, and "detail" prints out a short desctription about the error which occured
*/

#include <stdio.h>
#include <stdlib.h>

void DieWithUserMessage(const char *msg, const char *detail) 
{
	fputs(msg, stderr);	//prints the msg parameter to the terminal
	fputs(": ", stderr);	//prints ":", used for clarity in the terminal
	fputs(detail, stderr);	//prints the error message
	fputc('\n', stderr);	//prints a new line in the terminal, used for clarity
	exit(1);		//termination		
}


void DieWithSystemMessage(const char *msg) 
{
	perror(msg);	//print error message to stderr
	exit(1);	//termination
}
