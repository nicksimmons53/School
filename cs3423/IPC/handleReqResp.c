#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define BLKSIZE 1024
#define MAX_NAME 20
#define FIFO_PERMS (S_IRWXU | S_IRGRP| S_IROTH)
void handleRequests(int iFromFd, int iToFd) {
    char szBuf[BLKSIZE+1];         // general buffer for the requests 
    int iBytesRead, iBytesWritten; 
    char szClientNm[MAX_NAME+1];   // Name passed from client
    char szClientFIFO[MAX_NAME+1]; // Name of response FIFO
    int iFifoRespFd;               // Response FIFO File Descriptor idx
    int iLen;                  
    int iScanfCnt;                 // used by sscanf
    while (1) {
        // Read the request from the client
        if ((iBytesRead = read(iFromFd, szBuf, BLKSIZE)) <= 0)
            break;
        // write it to a log
        if ((iBytesWritten = write(iToFd, szBuf, iBytesRead)) == -1)
            break;
        // create the name of the response FIFO
        iScanfCnt = sscanf(szBuf, "%20s", szClientNm);
        if (iScanfCnt != 1)
            errExit("Invalid client name: '%s'", szBuf);
        snprintf(szClientFIFO, MAX_NAME, "%sPipe", szClientNm);
        // create the response FIFO
        if ((mkfifo(szClientFIFO, FIFO_PERMS) == -1) && (errno != EEXIST)) 
            errExit("Server failed to create a response FIFO named '%s'", szClientFIFO);
        // open the response FIFO as read/write     
        if ((iFifoRespFd = open(szClientFIFO, O_RDWR)) == -1) 
            errExit("Server failed to open response FIFO '%s'", szClientFIFO);
        // Build the response and write it to the FIFO
        snprintf(szBuf, BLKSIZE, "Response to %s, message read", szClientNm);
        iLen = strlen(szBuf)+1;
        if (write(iFifoRespFd, szBuf, iLen) != iLen) 
            errExit("Client write error to FIFO '%s'", szClientFIFO);
        close(iFifoRespFd);
    }
}
