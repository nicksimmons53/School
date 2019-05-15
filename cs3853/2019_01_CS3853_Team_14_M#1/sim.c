 // Group 14 - Nick Simmons, Christina Lew, Elliott Soler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFSIZE 100

void printOutput(int argc, char *argv[ ]);
int parseFile(char *file);

int main(int argc, char *argv[ ]) {
	// Check for proper num of input parameters
	if (argc != 11) {
		fprintf(stderr, "USAGE: ./sim -f traceFile -s cacheSize -b blockSize -a associativity -r replacementPolicy\n");
		return -1;
	}

	parseFile(argv[2]);				// Parse the trace file
	printOutput(argc, argv);		// Print output

	return 0;
}

// int parseFile(char *file)
// Purpose:		Parse the trace file provided line by line
// Parameters:	char	*file		pointer to the file name being parsed
// Returns:		0		successful
//				-1		error
int parseFile(char *file) {
	FILE *in;
	char buffer[BUFSIZE];
	int i = 0;

	// Open the file, read only, if NULL return error
	in = fopen(file, "r");
	if (in == NULL) {
		printf("ERROR: unable to open trace file\n");
		return -1;
	}

	// Read the line into a buffer
	while (fgets(buffer, BUFSIZE, in) != NULL) {
		char hexStr[9];
		char mem[3];
		unsigned long hexAddress;
		int bytesRead;

		if (strcmp(buffer, "\r\n") == 0)
			continue;

		// If the line is an EIP parse it and pull the address and read bytes
		if (strncmp(buffer, "EIP", 3) == 0) {

			// Hex address conversion
			strncpy(hexStr, buffer+10, 8);
			hexStr[8] = '\0';
			hexAddress = strtol(hexStr, NULL, 16);

			// Read memory conversion
			strncpy(mem, buffer+6, 2);
			mem[2] = '\0';
			bytesRead = strtod(mem, NULL);

			continue;
		}

		// MILESTONE 2 //

		char writeStr[9];
		char readStr[9];
		unsigned long writeAddress = 0;
		unsigned long readAddress = 0;

		// If the line starts with dstM parse it and pull the address
		if ( (strncmp(buffer, "dstM", 4) == 0) && (strncmp(buffer+6, "00000000", 8) != 0) ) {
			strncpy(writeStr, buffer+6, 8);
			writeStr[8] = '\0';
			writeAddress = strtol(writeStr, NULL, 16);
		}

		// If the line has srcM parse it and pull address
		if ( (strncmp(buffer+27, "srcM", 4) == 0) && (strncmp(buffer+33, "00000000", 8) != 0) ) {
			strncpy(readStr, buffer+33, 8);
			readStr[8] = '\0';
			readAddress = strtol(readStr, NULL, 16);
		}

		// Print out instructions
		if (writeAddress == 0 && readAddress == 0)
			printf("Address: 0x%08lx, length = %d. No data writes/reads occurred.\n", hexAddress, bytesRead);
		else if (writeAddress == 0 && readAddress != 0)
			printf("Address: 0x%08lx, length = %d. Data read at 0x%08lx, length = 4 bytes.\n", hexAddress, bytesRead, readAddress);
		else if (readAddress == 0 && writeAddress != 0)
			printf("Address: 0x%08lx, length = %d. Data write at 0x%08lx, length = 4 bytes.\n", hexAddress, bytesRead, writeAddress);
		else
			printf("Address: 0x%08lx, length = %d. Data write at 0x%08lx, length = 4 bytes, data read at 0x%08lx.\n", hexAddress, bytesRead, writeAddress, readAddress);
	}

	// Close the file, return
	fclose(in);
	return 0;
}

// void printOutput(int argc, char *argv[ ])
// Purpose:		This function will print the header section according to the command line args
// Parameters: 	int 	argc		number of input parameters
//				char	*argv[ ]	array of input parameters
// Returns:		void
void printOutput(int argc, char *argv[ ]) {
	int i;
	double cacheSize = strtod(argv[4], NULL);
	double blockSize = strtod(argv[6], NULL);
	double numOfBlocks = cacheSize / blockSize * 1024;
	double offsetBits = log2((double) blockSize);

	// Check for full associativity
	double associativity = strtod(argv[8], NULL);
	double indexBits;
	if ( (int)associativity == (int) log2(numOfBlocks))
		indexBits = 0;
	else
		indexBits = log2((cacheSize * pow((double) 2, (double) 10))/(pow((double) 2, offsetBits) * associativity));

	double tagBits = (int) 32 - offsetBits - indexBits;
	double overhead = (tagBits + 1) * numOfBlocks / 8;
	double impMemSize = cacheSize * pow((double) 2, (double) 10) + overhead;

	// Print project and group number
	printf("\n");
	printf("Cache Simulator CS 3853 Spring 2019 - Group #14\n");
	printf("\n");

	// Print the command line args
	printf("Cmd Line: ");
	for (i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");

	// Print the general info
	printf("Trace File: %s\n", argv[2]);
	printf("Cache Size: %d KB\n", (int) cacheSize);
	printf("Block Size: %d bytes\n", (int) blockSize);

	// Check for direct set associativity
	if ((int) associativity == 1)
		printf("Associativity: direct\n");
	else
		printf("Associativity: %d-way\n", (int) associativity);

	printf("R-Policy: %s\n", argv[10]);
	printf("\n");

	// Reprint the general info
	printf("Cache Size: %d KB\n", (int) cacheSize);
	printf("Block Size: %d bytes\n", (int) blockSize);

	if ((int) associativity == 1)
		printf("Associativity: direct\n");
	else
		printf("Associativity: %d\n", (int) associativity);

	printf("Policy: %s\n", argv[10]);
	printf("\n");


	// Print the calculated values
	printf("----- Calculated Values -----\n");

	if ((int)numOfBlocks%1024 == 0)
		printf("Total #Blocks: %dK (2^%d)\n", (int) numOfBlocks/1024, (int) log2(numOfBlocks));
	else
		printf("Total #Blocks: %d (2^%d)\n", (int) numOfBlocks, (int) log2(numOfBlocks));

	printf("Tag Size: %d bits\n", (int) tagBits);
	printf("Index Size: %d bits, Total Indices: %d K\n", (int) indexBits, (int) pow((double) 2, indexBits)/1024);
	printf("Overhead Size: %d bytes\n", (int) overhead);
	printf("Implementation Memory Size: %d bytes\n", (int) impMemSize);
	printf("\n");

	// Print the results of the project, not known as of Milestone 1
	printf("----- Results -----\n");
	printf("Cache Hit Rate: %%\n");
	printf("Cache Miss Rate: %%\n");
}
