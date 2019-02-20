// Nicholas Simmons srz888

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TARGET_PORT 47007

int main(void) {
	struct sockaddr_in targetAddr;
	struct sockaddr_in sourceAddr;
	char *targetIP = "67.11.114.92";
	char *message = "srz888 Nicholas Simmons\000";
	int sockFD;
	int i;
	int numBytes;
	int flags;
	int size;

	// Target sockaddr_in declarations
	targetAddr.sin_family = AF_INET;

	// Source sockaddr_in declarations (family, port, address)
	sourceAddr.sin_family = AF_INET;
	sourceAddr.sin_port = htons(0);
	sourceAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Creation of socket, STDERR if failure
	sockFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockFD < 0) {
		fprintf(stderr, "Sock Result: %d\n", sockFD);
		return -1;
	}

	// Use bind for source to find unused port, STDERR if failure
	i = bind(sockFD, (struct sockaddr *) &sourceAddr, sizeof(sourceAddr));
	if (i < 0) {
		fprintf(stderr, "Bind Result: %d\n", i);
		return -1;
	}

	// Put the target address into network order
	targetAddr.sin_addr.s_addr = inet_addr(&targetIP[0]);
	targetAddr.sin_port = htons(TARGET_PORT);

	// Send the message, STDERR if failure
	numBytes = strlen(message);
	flags = 0;
	size = sendto(sockFD, (char *)message, numBytes, flags, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
	if (size < 0) {
		fprintf(stderr, "SendTo Result: %d\n", size);
		return -1;
	}

	close(sockFD);
	return 0;
}
