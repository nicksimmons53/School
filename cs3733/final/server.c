/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

main(argc, argv)
     int     argc;
     char    *argv[];
{
  struct  sockaddr_in sad; /* structure to hold server's address  */
  int     port;            /* protocol port number                */
  
  struct  sockaddr_in cad; /* structure to hold client's address  */
  int     alen;            /* length of address                   */
  
  int     welcomeSocket, connectionSocket; /* socket descriptors  */
  
  char    clientSentence[128]; 
  char    capitalizedSentence[128]; 
  char    buff[128];
  int     i, n;
  
  /* Check command-line argument for protocol port and extract   */
  /* port number if one is specified. Otherwise, give error      */
  if (argc > 1) {                /* if argument specified        */
    port = atoi(argv[1]);        /* convert argument to binary   */
  } else { 
    fprintf(stderr,"Usage: %s port-number\n",argv[0]);
    exit(1);
  }
  
  /* Create a socket */

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0); /* CREATE SOCKET */
  if (welcomeSocket < 0) {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }
  
  /* Bind a local address to the socket */
  
  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address   */
  sad.sin_port = htons((u_short)port);/* set the port number        */ 
  
  if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"bind failed\n");
    exit(1);
  }
  
  /* Specify the size of request queue */
  
  if (listen(welcomeSocket, 10) < 0) {
    fprintf(stderr,"listen failed\n");
    exit(1);
  }
  
  /* Main server loop - accept and handle requests */
  
  while (1) {
    alen = sizeof(cad);
    if ( (connectionSocket=accept(welcomeSocket, (struct sockaddr *)&cad, &alen)) < 0) {
      fprintf(stderr, "accept failed\n");
      exit(1);
    }
    
    /* Get the sentence from the client */
    
    clientSentence[0]='\0';
    n=read(connectionSocket,buff,sizeof(buff));
    while(n > 0){
      strncat(clientSentence,buff,n);
      if (buff[n-1]=='\0') break;
      n=read(connectionSocket,buff,sizeof(buff));
    }
    
    /* Capitalize the sentence */
    
    for(i=0; i <= strlen(clientSentence); i++){
      if((clientSentence[i] >= 'a') && (clientSentence[i] <= 'z'))
	capitalizedSentence[i] = clientSentence[i] + ('A' - 'a');
      else
	capitalizedSentence[i] = clientSentence[i];
    }
    
    /* send it to the client */
    
    write(connectionSocket, capitalizedSentence, strlen(capitalizedSentence)+1);
    
    close(connectionSocket);
  }
}





