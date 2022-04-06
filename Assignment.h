#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

# define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))	// description
# define MAXLEN 128						// Max string size
# define BUFSIZE 512						// Max size of buffer
# define STDIN 0						// file decriptor for standard input

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);

// Handle error with sys msg
void DieWithSystemMessage(const char *msg);

// Play the game of hangman
void play_hangman (int in, int out);
#endif // PRACTICAL_H_
