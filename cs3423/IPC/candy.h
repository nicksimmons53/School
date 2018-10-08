// candy.h
#define READY 'R'
#define NOT_READY 'N'
#define CLIENT_FULL 'C'
#define BUF_SZ 100
#define TRUE 1
#define FALSE 0
int msleep (unsigned int msec);
#define SHM_NM "/tmp/abc123.sharedMemory"
typedef struct Data
{
    char cLockVar;      // Used to synchronize client and server
                        // which should be READY, NOT_READY or
                        // CLIENT_FULL.
    char szCandy[23];   // Candy provided by server
    int  iQty;          // Number of pieces of candy
} Data;