#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <string.h>
void errExit(const char szFmt[], ... );
int main(int argc, char *argv[])
{
    struct stat statBuf;
    int rc;
    // check the number of arguments
    if (argc < 2)
        errExit("Too few arguments for mystat\n");
    rc = stat (argv[1], &statBuf);
    if (rc < 0)
        errExit("stat: %s", strerror(errno));
    printf("stat information for '%s'\n", argv[1]);
    printf("dev is %ld\n", statBuf.st_dev);
    printf("inode is %ld\n", statBuf.st_ino);
    printf("mode is %o\n", statBuf.st_mode&07777);
    printf("nlink is %ld\n", statBuf.st_nlink);
    printf("uid is %d\n", statBuf.st_uid);
    printf("gid is %d\n", statBuf.st_gid);
    printf("total size is %ld\n", statBuf.st_size);
    printf("device preferred blksize is %lu\n", statBuf.st_blksize);
    printf("number of 512 blocks is %ld\n", statBuf.st_blocks);
    printf("last accessed at %s", ctime(&statBuf.st_atime));
    printf("last modified at %s", ctime(&statBuf.st_mtime));
    printf("last status change at %s", ctime(&statBuf.st_ctime));
    return 0;
}
