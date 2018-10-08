#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)
int main (int argc, char *argv[]) {
    int iFifoFd;            
    if (argc != 2)     //pipeS namedPipe 
       errExit("Usage: %s namedPipe > logfile\n", argv[0]);
    // create a named pipe to handle incoming requests
    if ((mkfifo(argv[FIFOARG], FIFO_PERMS) == -1) && (errno != EEXIST)) 
       errExit("Server failed to create a FIFO named '%s'", argv[FIFOARG]);
    // open the FIFO as read/write so that pipe server won't 
    // terminate if there isn't anything to read
    if ((iFifoFd = open(argv[FIFOARG], O_RDWR)) == -1) 
       errExit("Server failed to open FIFO '%s'", argv[FIFOARG]);
    // Handle all the requests
    handleRequests(iFifoFd, STDOUT_FILENO); 
    return 1;  // should not reach here, so return failure 
}
