#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#define BLKSIZE 1024
#define FIFOARG 1
int main (int argc, char *argv[]) {
   time_t timeCurrent;           
   int iLen;
   char szBuf[BLKSIZE+1];         // general buffer for the requests
   int iFifoFd;             
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
   if (write(iFifoFd, szBuf, iLen) != iLen) 
      errExit("Client write error to FIFO '%s'", argv[FIFOARG]);
   close(iFifoFd);
   return 0; 
}
