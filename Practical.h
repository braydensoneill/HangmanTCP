 /* Header file for overall hangman assignment */
 /* File: Assignment.c */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

// Generic header files
#include <stdbool.h>
#include <stdio.h>

// Networking header files
#include <sys/socket.h>

//These are the variables used in this file
# define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))
# define MAXLEN 128							// Max string size
# define BUFSIZE 512							// Max size of buffer
# define STDIN 0							// file decriptor for standard input

// These are the functions used in this file
void DieWithUserMessage(const char *msg, const char *detail);	// Handle error with user msg
void DieWithSystemMessage(const char *msg);				// Handle error with sys msg

#endif // ASSIGNMENT_H_
