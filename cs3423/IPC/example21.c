#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define REC_SZ 20
int main (int argc, char *argv[])
{
    long lForkPid;
    int iExitStatus = 0;    // Used by parent to get status of child
    long lWaitPid;          // PID of child that terminated
    int iReadFd, iWriteFd;  // File Descriptors for reader and writer
    int iReadSz;            // -1 (error) or size returned by read
    int iWriteSz;           // size to write
    int fdM[2];             // contains file descriptors for the pipe
    char szInput[REC_SZ+1]; // buffer for reader  
    char szOut[REC_SZ];     // buffer for writer
    int i;
    // create the pipe
    if (pipe(fdM) == -1)
        errExit("pipe not created: %s", strerror(errno));
    iReadFd = fdM[0];       // reader file descriptor
    iWriteFd = fdM[1];      // writer file descriptor
    lForkPid = fork();      // create a child process 
    // Both the parent and child continue here
    switch(lForkPid)
    {
        case -1:
            errExit("fork failed: %s", strerror(errno));
            break;
        case 0:  // child process - the reader
            close(iWriteFd);  // reader doesn't need writer FD
            iReadSz = read(iReadFd, szInput, REC_SZ);
            while (iReadSz > 0)
            {
                szInput[iReadSz] = '\0';
                printf("reader received: '%s'\n", szInput);
                iReadSz = read(iReadFd, szInput, REC_SZ);
            }
            if (iReadSz == -1)
                errExit("read error: %s", strerror(errno));
            close(iReadFd);
            break;
        default: // parent process - the writer
            close(iReadFd); // writer doesn't need reader FD
            for (i=1; i < argc; i++)
            {
                memset(szOut, 0, REC_SZ);
                iWriteSz = strlen(argv[i])> REC_SZ ? 
                       REC_SZ : strlen(argv[i]);
                memcpy(szOut, argv[i], iWriteSz); 
                write(iWriteFd, szOut, REC_SZ);
            }
            close (iWriteFd);
            lWaitPid = wait (&iExitStatus);
    }
    return 0;
}
