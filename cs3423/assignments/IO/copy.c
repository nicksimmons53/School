#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
void errExit(const char szFmt[], ... );
#define BLOCK_SZ 4096L
int main(int argc, char *argv[])
{
    int iFdRd;              // FD for file to be read
    int iFdWr;              // FD for file to be written
    char sbBuff[BLOCK_SZ];  // buffer for copying data
    int iRead;              // number of bytes read
    int iWrite;             // number of bytes written
    int iTotal = 0;         // total number of bytes written
    // check the number of arguments
    if (argc < 3)
        errExit("Too few arguments for copy");
    // Open the file to read
    iFdRd = open(argv[1], O_RDONLY);
    if (iFdRd < 0)
        errExit("Open file for read: %s", strerror(errno));
    // Open the file for create and give an error
    // if it exists.  Give permissions 00660
    iFdWr = open(argv[2], O_WRONLY|O_CREAT|O_EXCL
       , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if (iFdWr < 0)
        errExit("Open file for write: %s", strerror(errno));
    printf("Files:\nFD Filename\n");
    printf("%2d %s\n", iFdRd, argv[1]);
    printf("%2d %s\n", iFdWr, argv[2]);
    // read until EOF (iRead will be 0)
    while ((iRead = read(iFdRd, sbBuff, BLOCK_SZ)) > 0)
    {
        iWrite = write(iFdWr, sbBuff, iRead);
        if (iWrite < 0)
            errExit("writing copy: %s", strerror(errno));
        iTotal += iWrite;
        printf ("Read %d bytes, wrote %d bytes\n", iRead, iWrite);
    }
    close(iFdRd);
    close(iFdWr);
    printf("Total bytes written = %d bytes\n", iTotal);
    return 0;
}
