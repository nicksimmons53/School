#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
void errExit(const char szFmt[], ... ); // prototype
int main()
{
	int sd; // socket descriptor
	int commSd; // communication socket descriptor
	struct sockaddr_in local; // local host
	struct sockaddr_in client; // remote client
	int clientSaSize; // client sockaddr size
	int bindStatus;
	char *message; // message for client
	char msgFrClient[1024]; // message from client
	int bytesReceived; // size of message from client

	// create the socket with the appropriate properties
	sd = socket(AF_INET, SOCK_STREAM, 0);

	// verify socket
	if (sd == -1)
		errExit("Failed to create socket");

	// set up sockaddr
	local.sin_family = AF_INET;
	local.sin_port = htons(8088); 
	local.sin_addr.s_addr = INADDR_ANY; // all available interfaces

	// bind the address
	bindStatus = bind(sd, (struct sockaddr *) &local, sizeof(local));
	if (bindStatus < 0)
		errExit("Failed to bind socket");  

	printf("Socket Bound!\n");

	// set socket to listen mode
	listen(sd, 1);

	printf("Listening for connection requests...\n");

	// accept a request from the queue and create a new socket
	// Note that the program will loop until a request is in the queue
	clientSaSize = sizeof(struct sockaddr_in);
	while((commSd = accept(sd, (struct sockaddr *) &client, 
					(socklen_t *) &clientSaSize)) != -1){

		printf("Connection Established!\n");

		// read message from client
		bytesReceived = recv(commSd, msgFrClient, 1024, 0);
		if(bytesReceived > 0)
			printf("Message from client: \n%s\n", msgFrClient);
		else
			printf("No message from client.\n");

		// send a message
		message = "Your message was received!";
		write(commSd, message, strlen(message) + 1);
		//send(commSd, message, strlen(message) + 1, 0);
	}
	close(sd);
	close(commSd);

	if (commSd == -1)
		errExit("Failed to create communication socket: ", strerror(errno));


	return 0;
}

