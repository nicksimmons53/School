// Nicholas Simmons srz888

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TARGET_PORT 47007

int main(void) {
	struct sockaddr_in server;
	struct sockaddr_in client;
	char buffer[100];
	int sockFD;
	int i;
	int size;
	socklen_t addrlen;

	// Save struct variables
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(TARGET_PORT);

	// Creation of socket, STDERR if failure
	sockFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockFD < 0) {
		fprintf(stderr, "Sock Result: %d\n", sockFD);
		return -1;
	}

	// Use bind for source to find unused port, STDERR if failure
	i = bind(sockFD, (struct sockaddr *) &server, sizeof(server));
	if (i < 0) {
		fprintf(stderr, "Bind Result: %d\n", i);
		return -1;
	}

	// Create the message buffer structure to store and receive the message
	addrlen = sizeof(client);
	size = recvfrom(sockFD, (char *)buffer, 100, 0, (struct sockaddr *) &client, &addrlen);

	// Print statement to test if message was received
    printf("%s\n", buffer);
	close(sockFD);
	return 0;
}
