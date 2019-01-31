#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "myio.h"

int main(int argc, char *argv[ ]) {
	if (argc != 3) {
		printf("ERROR: EXPECTED 3 ARGUMENTS\n");
		exit(-1);
	}

	FILE *infile = fopen(argv[1], "r");
	FILE *outfile = fopen(argv[2], "w");
	char str[100];
	char line[100];
	int i;

	while (fgets(str, 100, infile) != NULL) {
		for (i = 0; i < strlen(str); i++) {
			if (str[i] == ' ' && str[i + 1] == ' ') {
				continue;
			}

			line[i] = str[i];
		}

		printf("%s", line);
	}

	fclose(infile);
	fclose(outfile);
}
