#include <unistd.h>
#define BLKSIZE 1024
void handleRequests(int iFromFd, int iToFd) 
{
    char szBuf[BLKSIZE];                 // general buffer for the requests
    int iBytesRead, iBytesWritten;
    while (1) 
    {
        // Read the request from the client
        if ((iBytesRead = read(iFromFd, szBuf, BLKSIZE)) <= 0)
            break;     
        // write it to a log
        if ((iBytesWritten = write(iToFd, szBuf, iBytesRead)) == -1)
            break;
    }
}
