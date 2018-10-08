#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
void errExit(const char szFmt[], ... );
#define MAXKEYS 100
#define KEYSIZE 12
typedef struct 
{
    long lRoot;
    
} ControlStruc;
typedef struct
{
    int iNumEntries;
    char key[MAXKEYS][KEYSIZE];
    long lChild[MAXKEYS];
} BTreeNode;

int main()
{
    int iFd;
    int iRead;
    int iWrite;
    long lRBA;
    long lSeekResult;
    BTreeNode bTreeNode;
    ControlStruc controlStruc;
    memset(&bTreeNode, 0, sizeof(bTreeNode));
    memset(&controlStruc, 0, sizeof(controlStruc));
    iFd = open("btree.dat", O_RDWR|O_CREAT
        , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if (iFd < 0)
        errExit("Could not open B-Tree: %s", strerror(errno));
    iRead = read(iFd, &controlStruc, sizeof(ControlStruc));
    switch(iRead)
    {
        case 0:  // file is empty
            controlStruc.lRoot = 1;
            bTreeNode.iNumEntries = 0;
            iWrite = write(iFd, &controlStruc, sizeof(ControlStruc));
            if (iWrite < 0)
                errExit("write of control: %s", strerror(errno));
            lSeekResult = lseek(iFd, sizeof(BTreeNode), SEEK_SET);
            if (lSeekResult < 0)
                errExit("lseek to root: %s", strerror(errno));
            iWrite = write(iFd, &bTreeNode, sizeof(bTreeNode));
            if (iWrite < 0)
                errExit("write of root: %s", strerror(errno));
            break; // ignored since we exited
        case -1:
            errExit("B+Tree file read error: %s",  strerror(errno));
            break;
        default:  // B+tree already exists
            printf("root is at %ld\n", controlStruc.lRoot);
            lRBA = controlStruc.lRoot * sizeof(bTreeNode);
            lSeekResult = lseek(iFd, lRBA, SEEK_SET);
            if (lSeekResult < 0)
                errExit("lseek to root: %s", strerror(errno));
            // get the root node
            iRead = read(iFd, &bTreeNode, sizeof(BTreeNode));
            if (iRead == 0)
                errExit("Could not read root, zero bytes returned");
            else if (iRead < 0)
                errExit("Root read failed: %s", strerror(errno));
            printf("number of keys is %d\n", bTreeNode.iNumEntries);
    }
    close(iFd);
    return 0;
}
