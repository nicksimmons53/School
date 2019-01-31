#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[ ]) {
	int numOfFiles;
	int i;
	char *fileNames[argc];

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

	}

	fflush(stderr);
	return 0;
}
