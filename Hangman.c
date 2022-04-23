 /* Playing the hangman game */
 /* File: Hangman.c */

// Project header files
#include "Hangman.h"

void play_hangman (int in, int out)
{
	file_descriptor_in = in;
	file_descriptor_out = out;
	
 	writeHostInfo(out);
 	
 	setRandomWord();
 	initialisePartWord();
 	
	/* Get a letter from player guess */
 	while (game_state == 'I')
 	{
 		requestInput(file_descriptor_out);
 		
		readInput(file_descriptor_in);
 		
 		checkLetter();
 		
 		printPartWord();
 			
 		checkGameOver();
 	}
}

void writeHostInfo(int fd_o)
{
	/* Write information about the server to the client when the game is run */
	sprintf(outbuf, "Playing hangman on host: %d \n", servPort);
 	write(fd_o, outbuf, strlen (outbuf));
}

void setRandomWord()
{
	/* Pick a word at random from the list */
 	whole_word = word[rand() % NUM_OF_WORDS];
 	word_length = strlen(whole_word);
 	syslog (LOG_USER | LOG_INFO, "server chose hangman word %s", whole_word);
 	printf("	Word chosen: %s \n\n", whole_word);
}

void initialisePartWord()
{
	/* No letters are guessed Initially */
 	for (i = 0; i <word_length; i++)
 	{
 		part_word[i]='-';
 	}
}

void requestInput(int fd_o)
{
	/* part_word can only be added to outbuff as %hhu, but it didn't work properly
	Made a new string variable it can be added as %s
	It works properly this way*/
 	char* currentAttempt = part_word;
	sprintf (outbuf, "\nLives: %d | Attempt: %s \nEnter a letter: ", lives, currentAttempt);
 	write (fd_o, outbuf, strlen(outbuf));
}

void readInput(int fd_i)
{
	// Read input from file_descriptor
	while (read (fd_i, guess, MAXLEN) <0) 
	{
 		if (errno != EINTR)
 			exit (4);
 		printf ("re-read the startin \n");
 	} /* Re-start read () if interrupted by signal */
 	sprintf(outbuf,"\n");
}

void checkLetter()
{
	/* Check the if the answer contains the guessed letter */
	
	good_guess = 0;
	
 	for (i = 0; i < word_length; i++) 
 	{
 		if (guess [0] == whole_word [i]) 
 		{
 			good_guess = 1;
 			part_word [i] = whole_word [i];
 		}
 	}
 	
 	if (!good_guess) 
 			lives--;
}

void checkGameOver()
{
	// If whole_word is equal to part_word, the player has won
	if (strcmp (whole_word, part_word) == 0) 
 		gameOverWin();
 		
 	// if the player has run out of lives, they have lost
 	else if (lives == 0) 
 		gameOverLose();
}

void gameOverWin()
{
	game_state = 'W'; /* W ==> User Won */
 	printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
 	printf("	Game Over\n");
	printf("	State: victory\n");
 	printf("	Attempt: %s\n", part_word);
 	printf("	Answer: %s\n\n", whole_word);
}

void gameOverLose()
{
	game_state = 'L'; /* L ==> User Lost */
 	printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
 	printf("	Game Over\n");
 	printf("	State: defeat\n");
 	printf("	Attempt: %s\n", part_word);
 	printf("	Answer: %s\n\n", whole_word);
}

void printPartWord()
{
 	printf("(%s-%d)\n", clientAddress, ntohs(client.sin_port));
 	printf("	Client has input a letter\n");
 	printf("	Letter: %s", guess);
 	printf("	Attempt: %s\n", part_word);
 	printf("	Answer: %s\n\n", whole_word);
}
