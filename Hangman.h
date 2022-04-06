 /* Playing the hangman game */
 /* File: Hangman.c */

#ifndef HANGMAN_H_
#define HANGMAN_H_

// Generic header files
#include <stdio.h>
#include <stdlib.h>

// Project header files
#include "Assignment.h"
#include "Server.h"

// These are the variables used in this file
# define maxlives 12;
unsigned char * whole_word, part_word [MAXLEN], guess[MAXLEN], outbuf [MAXLEN];
int file_descriptor_in;
int file_descriptor_out;
int lives = maxlives;
int game_state = 'I'; //I = Incomplete
int i;
int good_guess;
int word_length;
//unsigned char hostname[MAXLEN];

//List of words used in the hangman game, read from 'words' text file
char *word [] = {
	# include "words"
};

// Play the game of hangman
void play_hangman (int in, int out);
void writeHostInfo();
void setRandomWord();
void initialisePartWord();
void requestInput(int fd_o);
void readInput(int fd_i);
void checkLetter();
void checkGameOver();
void gameOverWin();
void gameOverLose();

#endif // HANGMAN_H_

