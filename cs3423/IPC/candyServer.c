// candy server
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "candy.h"
int main()
{
    Data *pData;
    key_t keySharedMem;
    int ishmid;
    FILE *pFile = fopen("candy.txt", "r");
    char szInputBuffer[BUF_SZ+1];
    int iScanfCount;
    if (pFile == NULL)
        errExit("File '%s' could not be opened", "candy.txt");
    // get a key for for independent processes to use 
    keySharedMem = ftok(SHM_NM, 1);
    if ( (key_t) keySharedMem == -1)
        errExit("Failed to derive key from '%s':%s", SHM_NM, strerror(errno));

    // create or get the shared memory using IPC_CREAT. 
    // If your process should only get it, don't specify IPC_CREAT.
    // Note that Data is just a typedef for data that we want in 
    // the shared memory.  It is something we have to define.
    ishmid = shmget(keySharedMem, sizeof(Data), IPC_CREAT
                    |S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if ( ishmid == -1 )
        errExit("Failed to get shared mem:", strerror(errno));

    // get a pointer to our Data 
    pData = shmat(ishmid, NULL, 0);
    // On failure we have to check if the pointer is -1.  Since this isn't
    // a numeric type, we have to compare it with a type cast of -1.
    if (pData == (void *)-1)
        errExit("Failed to attach shared mem:", strerror(errno));

    // Mark the shared data as not ready for consumption
    pData->cLockVar = NOT_READY;
    while (fgets(szInputBuffer, BUF_SZ, pFile) != NULL)
    {
        // copy the data to the target variables
        iScanfCount = sscanf(szInputBuffer, "%d %22[^\n]"
            , &pData->iQty
            , pData->szCandy);
        if (iScanfCount < 2)
            errExit("Only received %d valid values.  Found : %s\n"
                    , iScanfCount, szInputBuffer);
        // mark it as ready for consumption
        printf("Server: made %d %s ready\n", pData->iQty, pData->szCandy);
        pData->cLockVar = READY;
        // wait for client to consume it and mark it NOT_READY
        while (pData->cLockVar == READY)
            msleep(100); // sleep for 100 microsec
        // See if client is full of candy
        if (pData-> cLockVar == CLIENT_FULL)
        {
            printf("Server: client reported that it is FULL\n");
            break;
        }
    }
    // detach the shared memory
    if (shmdt(pData) == -1)
        errExit("Detach Failed: %s", strerror(errno));
    // deallocate the shared memory segment
    if (shmctl(ishmid, IPC_RMID, NULL) == -1)
        errExit("Removal of shared memory failed: %s", strerror(errno));
    return 0;
}

