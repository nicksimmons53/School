/**********************************************************************
    cs3743p2Driver.c by Larry Clark
    Copyright 2018 Larry Clark, this code may not be copied to any 
    other website.
Purpose:
    This program demonstrates a Hash File which uses overflow chaining.
    It uses a stream input command file to test the various 
    functions written by the students.
Command Parameters:
    p2 < infile > outfile
    The stream input file contains commands.  There are commands 
    which specify create, insert, read, delete, update, and dump.
   
    Commands in the input file:
    
    * comment       
        This is just a comment which helps explain what is being tested.
    
    CREATE MOVIE fileName numPrimary   
        Create the specified hash file if it doesn't already exist
        using hashCreate.
        The file will have numPrimary records in the primary area.
        Record 0 contains information about this hash file.
        The size of each record is the size of a Movie structure.   
    OPEN MOVIE fileName 
        Opens the specified file using hashOpen.  Place the returned
        FILE pointer in the driver's HashFile structure.
    INSERT MOVIE movieId,title,genre,rating,minutes
        Uses sscanf to get those attributes and populate a MOVIE structure.
        It invokes movieInsert to insert the movie into the hash file.
    READ MOVIE movieId
        Invokes movieRead to read the movie from the hash file and prints
        that movie including a returned RBN.
    PRINTALL MOVIE fileName
        Prints the contents of the specified file.
    NUKE MOVIE fileName
        Removes the specified file.
    UPDATE MOVIE movieId,title,genre,rating,minutes
        Uses sscanf to get those attributes and populate a MOVIE structure.
        It invokes movieUpdate to update the movie in the hash file.
    DELETE MOVIE movieId
        Invokes movieDelete to delete the movie in the hash file.  This will leave
        an empty entry.  This is for extra credit.
Results:
    All output is written to stdout.
    Prints each of the commands and their command parameters.  Some of the commands
    print information:  
        CREATE - prints the record size
        INSERT - prints the primary RBN
        READ   - prints the movie information (if found)
        PRINTALL- prints the file's contents
Returns:
    0       Normal
    99       Processing Error

Notes:
   
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "cs3743p2.h"

#define MAX_TOKEN_SIZE 50		// largest token size for tokenizer
#define MAX_BUFFER_SZ 200       // size of input buffer for command file
#define ERROR_PROCESSING 99

// prototypes only used by the driver
void processCommands(FILE *pfileCommand);
int getSimpleToken(char szInput[], const char szDelims[]
    , int *piBufferPosition, char szToken[]);
void printRC(int rc);

int main(int argc, char *argv[])
{
    // Process commands for manipulating hash files
    processCommands(stdin);
    printf("\n");
    return 0;
}

/******************** processCommands **************************************
    void processCommands(FILE *pfileCommand)
Purpose:
    Reads the Command file to process commands.  There are several types of
    records (see the program header for more information).
Parameters:
    I FILE *pfileCommand    command stream input
Notes:
    This calls functions inside:
        cs3723p2 
**************************************************************************/
void processCommands(FILE *pfileCommand)
{
    // variables for command processing
    char szInputBuffer[MAX_BUFFER_SZ+1];    // input buffer for a single text line
    char szCommand[MAX_TOKEN_SIZE + 1];     // command
    int bGotToken;                          // TRUE if getSimpleToken got a token
    int iBufferPosition;                    // This is used by getSimpleToken to 
                                            // track parsing position within input buffer
    int iScanfCnt;                          // number of values returned by sscanf
    int rc;                                 // return code from functions
    Movie movie;                              // Movie structure used by functions
    HashFile hashFile;                      // hash file 
    char szHashContent[11];                 // content type for hash file (e.g., MOVIE)
    char szFileNm[31];                      // Hash File Nm
    int iRBN;                               // RBN of a read movie

    // Initialize the hash File structure with null information.
    hashFile.pFile = NULL;
    memset(&hashFile.hashHeader, 0, sizeof(HashHeader));

    //  get command data until EOF
    while (fgets(szInputBuffer, MAX_BUFFER_SZ, pfileCommand) != NULL)
    {
        // if the line is just a line feed, ignore it
        if (szInputBuffer[0] == '\n')
            continue;

        // get the command
        iBufferPosition = 0;                // reset buffer position
        bGotToken = getSimpleToken(szInputBuffer, " \n\r", &iBufferPosition, szCommand);
        if (! bGotToken)
            errExit("Invalid stream input record:\n%s", szInputBuffer);

        // see if the command is a comment
        if (szCommand[0]== '*')
        {
            printf("%s", szInputBuffer);
            continue;       // it was just a comment
        }
        printf(" >> %s", szInputBuffer);

        // Process each type of input command
        if (strcmp(szCommand, "CREATE") == 0)
        { // CREATE MOVIE fileName numPrimary 
            memset(hashFile.hashHeader.sbFiller, 0
                , sizeof(hashFile.hashHeader.sbFiller));
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %30s %d"
                , szHashContent
                , szFileNm
                , &hashFile.hashHeader.iNumPrimary);
            if (iScanfCnt <  3)
                errExit("Invalid input:\n%s", szInputBuffer);
            hashFile.hashHeader.iRecSize = sizeof(Movie);
            printf("    Record size is %d\n", hashFile.hashHeader.iRecSize);
            rc = hashCreate(szFileNm, &hashFile.hashHeader);
            if (rc != 0)
                printRC(rc);
        }
        else if (strcmp(szCommand, "OPEN") == 0)
        { // OPEN MOVIE fileName 
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %30s"
                , szHashContent
                , szFileNm);
            if (iScanfCnt < 2)
                errExit("Invalid input:\n%s", szInputBuffer);
            hashFile.hashHeader.iRecSize = sizeof(Movie);
            rc = hashOpen(szFileNm, &hashFile);
            if (rc != 0)
                printRC(rc);
        }
        else if (strcmp(szCommand, "INSERT") == 0)
        { // INSERT MOVIE movieId,title,publisher,subject,pages
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %11[^,],%31[^,],%11[^,],%4[^,],%d"
                , szHashContent
                , movie.szMovieId
                , movie.szTitle
                , movie.szGenre
                , movie.szRating
                , &movie.iMinutes);
            if (iScanfCnt <  6)
                errExit("Invalid input:\n%s", szInputBuffer);
            printf("            Hash RBN is %d\n", hash(movie.szMovieId
                , hashFile.hashHeader.iNumPrimary));
            rc = movieInsert(&hashFile, &movie);
            if (rc != 0)
                printRC(rc);
        }
        else if (strcmp(szCommand, "READ") == 0)
        { // READ MOVIE movieId
            memset(&movie, 0, sizeof(movie));
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %11s"
                , szHashContent
                , movie.szMovieId);
            if (iScanfCnt <  2)
                errExit("Invalid input:\n%s", szInputBuffer);
            printf("            Hash RBN is %d\n", hash(movie.szMovieId
                , hashFile.hashHeader.iNumPrimary));
            rc = movieRead(&hashFile,&movie, &iRBN);
            if (rc != 0)
                printRC(rc);
            else
            {
                printf("    %2d", iRBN);  //indent like the DUMP command
                printMovie(&movie);
            }
        }

        else if (strcmp(szCommand, "UPDATE") == 0)
        { // UPDATE MOVIE movieId,title,publisher,subject,pages
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %11[^,],%31[^,],%11[^,],%4[^,],%d"
                , szHashContent
                , movie.szMovieId
                , movie.szTitle
                , movie.szGenre
                , movie.szRating
                , &movie.iMinutes);
            if (iScanfCnt <  6)
                errExit("Invalid input:\n%s", szInputBuffer);
            printf("            Hash RBN is %d\n", hash(movie.szMovieId
                , hashFile.hashHeader.iNumPrimary));
            rc = movieUpdate(&hashFile,&movie);
            if (rc != 0)
                printRC(rc);
        }
        
        else if (strcmp(szCommand, "DELETE") == 0)
        { // DELETE MOVIE movieId
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %11s"
                , szHashContent
                , movie.szMovieId);
            if (iScanfCnt <  2)
                errExit("Invalid input:\n%s", szInputBuffer);
            rc = movieDelete(&hashFile,&movie);
            if (rc != 0)
                printRC(rc);
        }
        else if (strcmp(szCommand, "PRINTALL") == 0)
        { // PRINTALL MOVIE fileName
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %30s"
                , szHashContent
                , szFileNm);
            if (iScanfCnt < 2)
                errExit("Invalid input:\n%s", szInputBuffer);
            // Flush the Hash File so that printAll can see it
            fflush(hashFile.pFile);
            rc = printAll(szFileNm);
            if (rc != 0)
                printRC(rc);
        }
        else if (strcmp(szCommand, "NUKE") == 0)
        { // NUKE MOVIE fileName
            iScanfCnt = sscanf(&szInputBuffer[iBufferPosition]
                , "%10s %30s"
                , szHashContent
                , szFileNm);
            if (iScanfCnt < 2)
                errExit("Invalid input:\n%s", szInputBuffer);
            rc = remove(szFileNm);   // Ignore the rc
        }

        else
            printf("   *** Unknown command in input: '%s'\n", szCommand);
    }
}
/******************** hash **************************************
  int hash(char szKey[],int iNumPrimary)
Purpose:
    Hashes a key to return an RBN between 1 and iNumPrimary 
    (inclusive).
Parameters:
    I   char szKey[]              key to be hashed
    I   iNumPrimary               number of primary records
Returns:
    Returns a hash value into the primary area.  This value is between 1
    and pHashHeader->iNumPrimary
Notes:
    - Primary area of the hash table has subscripts from 1 to 
      iNumPrimary.
    - Overflow area begins with iNumPrimary + 1.
    - The hash function is average at spreading the values.  
**************************************************************************/
int hash(char szKey[],int iNumPrimary)
{
    int iHash = 0;
    int i;
    if (iNumPrimary <= 0)
        errExit("hash function received an invalid iNumPrimary value: %d\n"
            , iNumPrimary);
    for (i = 0; i < (int) strlen(szKey); i++)
    {
        iHash += szKey[i];
    }
    // restrict it to the primary area
    iHash = abs(iHash) % iNumPrimary +1; 
    return iHash;
}

/******************** printAll **************************************
  int printAll(char szFileNm[]))
Purpose:
    Prints the contents of the specified hash file. 
Parameters:
    I   char szFileNm[]               hash file name
Returns:
    RC_OK - normal
    RC_FILE_NOT_FOUND - file not found
    RC_HEADER_NOT_FOUND - header record not found
    RC_BAD_REC_SIZE - bad record size
Notes:
    - Primary area of the hash table has subscripts from 1 to 
      hashHeader.iNumPrimary.
    - Overflow area begins with hashHeader.iNumPrimary + 1.
**************************************************************************/
int printAll(char szFileNm[])
{
    int rc;                                 // return code
    HashFile hashFile;                      // hash file structure containing
                                            // a FILE pointer and header.

    struct Record
    {
        int iNextChain;
        char sbBuffer[MAX_REC_SIZE];
    } record;

    long lRBA;                              // relative byte address
    long lRecNum;                           // record number
    int rcFseek;                            // fseek return code
    // open the hash file
    memset(&hashFile.hashHeader, 0, sizeof(HashHeader));
    hashFile.hashHeader.iRecSize = sizeof(Movie);
    rc = hashOpen(szFileNm, &hashFile);
    if (rc != 0)
        return rc;

    // print the Hash Header
    printf("    Primary=%d, HighOverflow=%d, RecSize=%d\n"
        , hashFile.hashHeader.iNumPrimary
        , hashFile.hashHeader.iHighOverflowRBN
        , hashFile.hashHeader.iRecSize);
    if (hashFile.hashHeader.iRecSize < sizeof(Movie))
        return RC_BAD_REC_SIZE;

    // locate the first record
    lRecNum = 1;
    lRBA = lRecNum*hashFile.hashHeader.iRecSize;
    rcFseek = fseek(hashFile.pFile, lRBA, SEEK_SET);
    assert(rcFseek == 0);
    while(fread(&record, hashFile.hashHeader.iRecSize
        , 1L, hashFile.pFile)==1)
    {
        if (lRecNum == hashFile.hashHeader.iNumPrimary+1)
            printf("Overflow:\n");
        if (record.iNextChain != 0 || record.sbBuffer[0] != '\0')
        {
            printf("    %2ld", lRecNum);
            printMovie((Movie *) &record);
        }
        lRecNum += 1;
    }
    fclose(hashFile.pFile);
    return RC_OK;
}
/******************** printMovie **************************************
    void printMovie(Movie *pMovie)
Purpose:
    Prints the movie information for one movie.
Parameters:
    I Movie *pMovie       // Movie info
**************************************************************************/
void printMovie(Movie *pMovie)
{
    printf(" Next=%-3d  ", pMovie->iNextChain);
    printf(" %11s %-31s %-11s %-4s %4d \n"
        , pMovie->szMovieId
        , pMovie->szTitle
        , pMovie->szGenre
        , pMovie->szRating
        , pMovie->iMinutes);
}


/******************** printRC **************************************
    void printRC(int rc)
Purpose:
    For a non-zero RC, it prints an appropriate message
Parameters:
    I int rc;           // return code value which is an error if
                        // non-zero

**************************************************************************/
void printRC(int rc)
{
    char *pszMsg;           // pointer to an error message
    char szBuf[100];        // buffer for building an error message
    switch(rc)
    {
        case RC_OK:
            return;
        case RC_FILE_EXISTS:
            pszMsg = "file already exists";
            break;
        case RC_REC_EXISTS:
            pszMsg = "record already exists";
            break;
        case RC_REC_NOT_FOUND:
            pszMsg = "record not found";
            break;
        case RC_INVALID_PARM:
            pszMsg = "invalid parameter";
            break;
        case RC_FILE_NOT_FOUND:
            pszMsg = "file not found or invalid info record";
            break;
        case RC_HEADER_NOT_FOUND:
            pszMsg = "header record not found";
            break;
        case RC_BAD_REC_SIZE:
            pszMsg = "invalid record size in info recordd";
            break;
        case RC_LOC_NOT_FOUND:
            pszMsg = "Location not found for read";
            break;
        case RC_LOC_NOT_WRITTEN:
            pszMsg = "Location not written for write";
            break;
        case RC_SYNONYM:
            pszMsg = "Synonym exists at the specified location";
            break;
        case RC_NOT_IMPLEMENTED:
            pszMsg = "Function not implemented";
            break;
        default:
            sprintf(szBuf, "unknown return code: %d", rc);
            pszMsg = szBuf;
    }
    printf("    **** ERROR: %s\n", pszMsg);
}
/******************** getSimpleToken **************************************
 int getSimpleToken(char szInput[], char szDelims[]
     , int *piBufferPosition, char szToken[]) 
Purpose:
    Returns the next token in a string.  The delimiter(s) for the token is  
    passed as a parameter.  To help positioning for a subsequent call, it 
    also returns the next position in the buffer.
Parameters:
    I   char szInput[]          input buffer to be parsed
    I   char szDelims[]         delimiters for tokens
    I/O int *piBufferPosition   Position to begin looking for the next token.
                                This is also used to return the next 
                                position to look for a token (which will
                                follow the delimiter).
    O   char szToken[]          Returned token.  
Returns:
    Functionally:
        TRUE - a token is returned
        FALSE - no more tokens
    iBufferPosition parm - the next position for parsing
    szToken parm - the returned token.  If not found, it will be an empty string.
Notes:
    - If the token is larger than MAX_TOKEN_SIZE, we return a truncated value.
**************************************************************************/

int getSimpleToken(char szInput[], const char szDelims[]
    , int *piBufferPosition, char szToken[]) 
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    
    // check for past the end of the string
    if (*piBufferPosition >= (int) strlen(szInput))
    {
        szToken[0] = '\0';              // mark it empty
        return FALSE;                   // no more tokens
    }

    // get the position of the first delim, relative to the iBufferPosition
    iDelimPos = strcspn(&szInput[*piBufferPosition], szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > MAX_TOKEN_SIZE)
        iCopy = MAX_TOKEN_SIZE;             // truncated size
    else
        iCopy = iDelimPos;
    
    // copy the token into the target token variable
    memcpy(szToken, &szInput[*piBufferPosition], iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    *piBufferPosition += iDelimPos + 1;  
    return TRUE;
}

/******************** errExit **************************************
  void errExit(const char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program.
Parameters:
    I   const char szFmt[]      This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Returns:
    Exits the program with a return code of ERROR_PROCESSING (99).
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Requires including <stdarg.h>
**************************************************************************/
void errExit(const char szFmt[], ... )
{
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
