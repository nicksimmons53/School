#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100
#define TRUE 1
#define FALSE 0

char *ReadLineFile(FILE *infile) {
    char buffer[BUFFER_SIZE];
    char *in;
    int i;
	int readStatus;
	int endPos = 0;
	int reads = 0;
    int nullFlag = FALSE;

	in = malloc(BUFFER_SIZE);
    // Loop through the file until broken out
    while (TRUE) {

        // Read the file into the buffer
        readStatus = read(0, &buffer, BUFFER_SIZE);

        // Loop through the chars in the buffer
        for (i = 0; i <= BUFFER_SIZE; i++) {

            // If the buffer is EOF or \n exit
            if (buffer[i] == EOF || buffer[i] == '\n') {
				readStatus = FALSE;
                break;
            } else {
                // Read the char from the buffer into the input
                in[i] = buffer[i];
                nullFlag = TRUE;
				endPos++;
            }
        }

		if (endPos == 0)
			return NULL;

		in[endPos] = '\0';
		in = realloc(in, endPos + 1);
		return(in);
    }
}

char *ReadLine( ) {
	printf("Please enter a line: \n");
    return(ReadLineFile(stdin));
}

int ReadInteger( ) {
	char *userInput;
	int num;
	int i;

	printf("Please enter an integer: \n");
	userInput = ReadLineFile(stdin);

	for (i = 0; i < strlen(userInput); i++) {
		if (userInput[i] < '0' || userInput[i] > '9' || userInput[i] == '.') {
			printf("ERROR INVALID INPUT\n");
			return -1;
		}
	}

	num = atoi(userInput);
	free(userInput);
	return num;
}

int ReadDouble( ) {
	char *userInput;
	char *ptr;
	double num;

	printf("Please enter a decimal digit: \n");
	userInput = ReadLineFile(stdin);
	char *check = strstr(userInput, ".");
	if (check == NULL) {
		printf("ERROR INVALID INPUT\n");
		return -1;
	}

	num = atof(userInput);

	free(userInput);
	return num;
}
