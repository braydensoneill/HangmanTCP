#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);

// Handle error with sys msg
void DieWithSystemMessage(const char *msg);

enum sizeConstants 
{
	MAXSTRINGLENGTH = 128,
	BUFSIZE = 512,
};

#endif // PRACTICAL_H_
