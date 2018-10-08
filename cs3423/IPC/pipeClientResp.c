#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#define FIFOARG 1
#define MAX_NAME 20
#define BLKSIZE 1024
int main (int argc, char *argv[]) {
    time_t timeCurrent;
    int iLen;
    char szBuf[BLKSIZE+1];         // general buffer for the requests
    int iFifoFd, iFifoRespFd;
    char szRespFifoNm[MAX_NAME+1]; // Response FIFO name 
    int iBytesRead;    
    if (argc != 3) // pipeC pipeNm
        errExit( "Usage: %s fifoName clientName\n", argv[0]);
    // open the named pipe
    if ((iFifoFd = open(argv[FIFOARG], O_WRONLY)) == -1) 
        errExit("Client could not open FIFO '%s'", argv[FIFOARG]);
    timeCurrent = time(NULL);
    snprintf(szBuf, PIPE_BUF, "%s %d: %s"
        , argv[2]
        , (int)getpid()
        , ctime(&timeCurrent));
    iLen = strlen(szBuf);
    // write a request to the server
    if (write(iFifoFd, szBuf, iLen) != iLen) 
        errExit("Client write error to FIFO '%s'", argv[FIFOARG]);
    // create the response FIFO name and open it
    snprintf(szRespFifoNm, MAX_NAME, "%sPipe", argv[2]);
    if ((iFifoRespFd = open(szRespFifoNm, O_RDWR)) == -1) 
        errExit("Client could not open response FIFO '%s'", szRespFifoNm);
    // read the response from the server
    if ((iBytesRead = read(iFifoRespFd, szBuf, BLKSIZE)) > 0)
        printf("client received message: %s\n", szBuf);
    close(iFifoFd);
    close(iFifoRespFd);
    return 0; 
}
