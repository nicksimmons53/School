#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int wordCount(char *fileName);

int main(int argc, char *argv[ ]) {
	int numOfFiles;
	int i;
	char *fileNames[argc];
	pid_t childPID = 0;
	long parentPID = getpid( );

	// Usage Error, exit if true
	if (argc <= 1) {
		fprintf(stderr, "USAGE: ./wordcount filename1 filename2 . . . filenameN\n");
		exit(-1);
	}

	// Find the number of files
	// Put the file names into an array
	numOfFiles = argc - 1;
	for (i = 1; i <= numOfFiles; i++) {
		fileNames[i - 1] = argv[i];
	}

	// Main process creation for each file
	for (i = 0; i < numOfFiles; i++) {
		int count;

		// Create child process for counting words
		if ((childPID = fork( )) <= 0)
			break;

		// Call to wordCount function
		count = wordCount(fileNames[i]);
		printf("Child process for %s: number of words is %d\n", fileNames[i], count);
	}

	// for loop for parent process to wait for child completion
	for ( ; ; ) {
		childPID = wait(NULL);
		if (childPID == -1 && errno != EINTR)
			break;
	}

	// If current process is parent process, complete the program
	if (parentPID == getpid( ))
		printf("All %d files have been counted!\n", numOfFiles);

	fflush(stderr);
	return 0;
}

// int wordCount(char *fileName)
//
// Task: 		wordCount will open a file and read it line by line and return the total count of
//				words in the file
// Parameter:	char *fileName		file name to be read
// Return:		int 				amount of words in the file
int wordCount(char *fileName) {
	FILE *file;
	char c;
	int wordCount = 0;
	int i;

	file = fopen(fileName, "r");
	while ((c = fgetc(file)) != EOF) {
		if (c == ' ' || c == '\n' || c == '\t')
			wordCount++;
	}

	fclose(file);
	return wordCount;
}
