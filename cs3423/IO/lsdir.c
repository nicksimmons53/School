#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
void errExit(const char szFmt[], ... );
int main(int argc, char *argv[])
{
    DIR *pDir;
    struct dirent *pDirent;
    if (argc < 2)
        errExit("too few arguments, directory name needed");
    pDir = opendir(argv[1]);
    if (pDir == NULL)
        errExit("opendir could not open '%s': %s", argv[1], strerror(errno));
    // show information about each directory entry
    printf("%-10s  %s\n", "INODE", "FILENAME");
    while ((pDirent = readdir(pDir)) != NULL)
    {
        printf("%10ld  %s\n", pDirent->d_ino, pDirent->d_name);
    }
    closedir(pDir);
    return 0;
}
