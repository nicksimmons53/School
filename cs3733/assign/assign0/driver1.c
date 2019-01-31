#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "myio.h"

int main(int argc, char *argv[ ]) {
	int numOfInts;
	int numOfDoubles;
	int numOfLines;
	int i;
	int j;
	double k;
	char *l;

	if (argc != 4) {
		printf("ERROR: Expected 4 arguments\n");
		exit(-1);
	}

	numOfInts = atoi(argv[1]);
	numOfDoubles = atoi(argv[2]);
	numOfLines = atoi(argv[3]);

	int intArray[numOfInts];
	double doubleArray[numOfDoubles];
	char *stringArray[numOfLines];

	for (i = 0; i < numOfInts; i++) {
		// Read ints
		j = ReadInteger( );
		intArray[i] = j;

		printf("%d\n", j);
	}

	for (i = 0; i < numOfDoubles; i++) {
		// Read doubles
		k = ReadDouble( );
		doubleArray[i] = k;

		printf("%f\n", k);
	}

	for (i = 0; i < numOfLines; i++) {
		// Read lines
		l = ReadLine( );
		stringArray[i] = l;

		printf("%s\n", l);
		free(l);
	}

	int intMax = intArray[0];
	for (i = 0; i < numOfInts; i++) {
		if (intArray[i] > intMax)
			intMax = intArray[i];
	}
	printf("Largest int is: %d\n", intMax);

	double doubleMax = doubleArray[0];
	for (i = 0; i < numOfDoubles; i++) {
		if (doubleArray[i] > doubleMax)
			doubleMax = doubleArray[i];
	}
	printf("Largest double is: %f\n", doubleMax);

	char *lineMax = stringArray[0];
	for (i = 0; i < numOfLines; i++) {
		if (strlen(stringArray[i]) > strlen(lineMax))
			lineMax = stringArray[i];
	}
	printf("Longest string is: %s\n", lineMax);

	return 0;
}
