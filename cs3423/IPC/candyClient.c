// candy client
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
    // get a key for for independent processes to use 
    keySharedMem = ftok(SHM_NM, 1);
    if ( (key_t) keySharedMem == -1)
        errExit("Failed to derive key from '%s': %s", SHM_NM, strerror(errno));

    // create or get the shared memory using IPC_CREAT. 
    // If your process should only get it, don't specify IPC_CREAT.
    // Note that Data is just a typedef for data that we want in 
    // the shared memory.  It is something we have to define.
    ishmid = shmget(keySharedMem, sizeof(Data), 
                    S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if ( ishmid == -1 )
        errExit("Failed to get shared mem:", strerror(errno));

    // get a pointer to our Data 
    pData = shmat(ishmid, NULL, 0);
    // On failure we have to check if the pointer is -1.  Since this isn't
    // a numeric type, we have to compare it with a type cast of -1.
    if (pData == (void *)-1)
        errExit("Failed to attach shared mem:", strerror(errno));

    int iTotal = 0;
    // Continue until client has gotten 100 pieces of candy.
    while (TRUE)
    {
        // wait until it is server says it is ready
        while (pData->cLockVar != READY)
            msleep(100); // sleep for 100 microsec
        // Consume the candy
        printf("Client: ate %d %s ready\n", pData->iQty, pData->szCandy);
        iTotal += pData->iQty;
        if (iTotal > 100)
            break;
        pData->cLockVar = NOT_READY;
    }
    printf("Client: full\n");
    pData->cLockVar = CLIENT_FULL;
    if (shmdt(pData) == -1)
        errExit("Detach Failed: %s", strerror(errno));
}

