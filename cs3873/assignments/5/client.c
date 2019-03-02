// Nicholas Simmons srz888

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TARGET_PORT 51019

int main(void) {
	struct sockaddr_in target;
	struct sockaddr_in source;
	char *targetIP = "67.11.114.92";
	char *message = "srz888 Nicholas Simmons";
	char buffer[100];
	int sock, b, c, sentBytes, recvBytes;

	// Create a socket
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		fprintf(stderr, "Socket creation error: %d\n", sock);
		return -1;
	}

	// Target PC declarations
	target.sin_family = AF_INET;
	target.sin_port = htons(TARGET_PORT);

	// Source PC declarations
	source.sin_family = AF_INET;
	source.sin_port = htons(0);
	source.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind the socket to the port
	b = bind(sock, (struct sockaddr *) &source, sizeof(source));
	if (b != 0) {
		fprintf(stderr, "Bind error: %d\n", b);
		return -1;
	}

	// Convert the address to 32 bit binary
	target.sin_addr.s_addr = inet_addr(&targetIP[0]);

	// Connect the socket
	c = connect(sock, (struct sockaddr *) &target, sizeof(target));
	if (c < 0) {
		fprintf(stderr, "Error connecting to target: %d\n", c);
		return -1;
	}

	// Send the message
	sentBytes = send(sock, message, strlen(message), 0);
	if (sentBytes < 0) {
		fprintf(stderr, "Error sending message: %d\n", sentBytes);
		close(sock);
		return -1;
	}

	// Check for more message to send

	// Receive reply from the server
	recvBytes = recv(sock, buffer, 256, 0);
	if (recvBytes < 0) {
		fprintf(stderr, "Error receiving message: %d\n", recvBytes);
		close(sock);
		return -1;
	}
	printf("Received Messsage: %s\n", buffer);

	// Close the socket
	fflush(stderr);
	close(sock);

	return 0;
}
