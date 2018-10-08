#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
void errExit(const char szFmt[], ... );
int main()
{
    int iFdRd;
    int iFdWr;
    // Open a file to read
    iFdRd = open("readIt.txt", O_RDONLY);
    if (iFdRd < 0)
        errExit("Open file for read: %s", strerror(errno));
    // Open the file for create and give an error
    // if it exists.  Give permissions 00660
    iFdWr = open("writeIt.txt", O_WRONLY|O_CREAT|O_EXCL
       , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if (iFdWr < 0)
        errExit("Open file for write: %s", strerror(errno));
    close(iFdRd);
    close(iFdWr);
    return 0;
}
