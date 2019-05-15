/* client.c - code for example client program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main (int argc, char *argv[ ]) {
	struct  sockaddr_in sad; /* structure to hold an IP address     */
	int     clientSocket;    /* socket descriptor                   */
	struct  hostent  *ptrh;  /* pointer to a host table entry       */

	char    *host;           /* pointer to host name                */
	int     port;            /* protocol port number                */

	char    Sentence[128];
	char    modifiedSentence[128];
	char    buff[128];
	int     n;

	if (argc != 3) {
		fprintf(stderr,"Usage: %s server-name port-number\n",argv[0]);
		exit(1);
	}

	/* Extract host-name from command-line argument */
	host = argv[1];         /* if host argument specified   */

	/* Extract port number  from command-line argument */
	port = atoi(argv[2]);   /* convert to binary            */

	/* Create a socket. */

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	/* Connect the socket to the specified server. */

	memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;           /* set family to Internet     */
	sad.sin_port = htons((u_short)port);
	ptrh = gethostbyname(host); /* Convert host name to equivalent IP address and copy to sad. */
	if ((char *)ptrh == NULL) {
		fprintf(stderr,"invalid host: %s\n", host);
		exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	if (connect(clientSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"connect failed\n");
		exit(1);
	}

	/* Read a sentence from user */

	printf("Sentence   :   "); gets(Sentence);

	/* Send the sentence to the server  */

	write(clientSocket, Sentence, strlen(Sentence)+1);

	/* Get the modified sentence from the server and write it to the screen*/
	modifiedSentence[0] = '\0';
	n = read(clientSocket, buff, sizeof(buff));
	while(n > 0) {
		strncat(modifiedSentence,buff,n);
		if (buff[n-1]=='\0') break;
		n = read(clientSocket, buff, sizeof(buff));
	}

	printf("Modified to -> %s\n", modifiedSentence);

	/* Close the socket. */
	close(clientSocket);

	return 0;
}
