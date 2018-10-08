#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "errExit.c"
void errExit(const char szFmt[], ... ); // prototype
#define REMOTE_HOST "129.115.28.4" // cs.utsa.edu

int main()
{
    int sd; // socket descriptor
    struct sockaddr_in host; // remote host to connect to
    int connectStatus;
    ssize_t bytesSent;
    ssize_t bytesReceived;
    char *request;
    char response[2048];
    
    // create the socket with the appropriate properties
    sd = socket(AF_INET, SOCK_STREAM, 0);
    
    // verify socket
    if (sd == -1)
          errExit("Failed to create socket");
    
    // set up sockaddr
    host.sin_family = AF_INET;
    host.sin_port = htons(80); // http
    inet_pton(AF_INET, REMOTE_HOST, &(host.sin_addr.s_addr));

     
    // Connect to remote server
    connectStatus = connect(sd, (struct sockaddr *) &host, 
            sizeof(host));
    if (connectStatus < 0)
        errExit("Failed to connect");     
        
    printf("Connection Established!\n");
    
    // make the request
    request = "GET / HTTP/1.0/\r\n\r\n";
    bytesSent = send(sd, request, strlen(request), 0);
    if(bytesSent != strlen(request) )
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
