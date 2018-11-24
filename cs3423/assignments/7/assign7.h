#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define ERROR_PROCESSING 99
#define BUFFER_SIZE 150
#define inventoryFileName "inventory.dat"
FILE *inventoryFile;

typedef struct {
    char itemName[64];
    char simpleName[16];
    int currentQuantity;
    int maxQuantity;
    char body[128];
} Item;

void errExit(const char szFmt[], ... ) {
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n");
    exit(ERROR_PROCESSING);
}

int switchCase(char choiceC[ ]);
int C( );
int R( );
int U( );
int D( );
