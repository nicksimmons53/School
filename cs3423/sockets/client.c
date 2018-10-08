#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "errExit.c"
void errExit(const char szFmt[], ... ); // prototype

int main(int argc, char *argv[]){
    int sd; // socket descriptor
    struct sockaddr_in host; // remote host to connect to
    int connectStatus;
    ssize_t bytesSent;
    ssize_t bytesReceived;
    char *request;
    char response[2048];

	if(argc != 4)
		errExit("usage: %s <remote-IP-address> <port> <message>", argv[0]);
    
    // create the socket with the appropriate properties
    sd = socket(AF_INET, SOCK_STREAM, 0);
    
    // verify socket
    if (sd == -1)
          errExit("Failed to create socket");
    
    // set up sockaddr
    host.sin_family = AF_INET;
    host.sin_port = htons(atoi(argv[2])); 
    inet_pton(AF_INET, argv[1], &(host.sin_addr.s_addr));

     
    // Connect to remote server
    connectStatus = connect(sd, (struct sockaddr *) &host, 
            sizeof(host));
    if (connectStatus < 0)
        errExit("Failed to connect");     
        
    printf("Connection Established!\n");
    
    // send the message
    request = argv[3];
    bytesSent = send(sd, request, strlen(request) + 1, 0);
    if(bytesSent != strlen(request) + 1)
        errExit("Failed to send request");     
    
    printf("Request Sent!\n");

    // read reply
    bytesReceived = recv(sd, response, sizeof(response), 0);
    if(bytesReceived < 0)
        errExit("Failed to receive response");     
    printf("Response:\n%s\n", response);
    
    close(sd);
    return 0;
}


