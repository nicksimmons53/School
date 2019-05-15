 /*************************************************************************
cs3743p1.c written by Nicholas Simmons (srz888)
Purpose:
    This program will create and open a Hash File to read and write
    binary records to a location in the hash file according to a hash
    function.
Command Parameters:
    make
    ./p1 < p1Input.txt
Input:
    Input from a text file read through cs3743p1Driver.c
Results:
    Return pointers to record structures being written for read and write.
Returns:
    Individual functions have returns to be read by the driver.
Notes:
    None.
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "cs3743p1.h"

/*********************** hashCreate **************************************
Purpose:
    Create a hash file and open it to write a header record to
    the beginning position in the file.
Parameters:
    char            szFileNm[ ]     File name for the file to be created
    HashHeader      *pHashHeader    HashHeader record to be written to
Notes:
    errExit if the hash file does not exist, but cannot be opened
Return Value:
    RC_FILE_EXISTS      File already exists
    RC_OK               Executes successfully
*************************************************************************/
int hashCreate(char szFileNm[ ], HashHeader *pHashHeader) {
    // Declare a new hashfile
    HashFile hashFile;

    // Check to see if the hash file already exists, exit if TRUE
    hashFile.pFile = fopen(szFileNm, "rb");
    if (hashFile.pFile != NULL)
        return RC_FILE_EXISTS;

    // Open and create the binary file, errExit if failed
    hashFile.pFile = fopen(szFileNm, "wb+");
    if (hashFile.pFile == NULL)
        errExit("Could not open new hash file: '%s'\n", szFileNm);

    // Assign iHighOverflowRBN to iNumPrimary
    pHashHeader->iHighOverflowRBN = pHashHeader->iNumPrimary;

    // Search and set the beginning position in the hashFile
    fseek(hashFile.pFile, 0, SEEK_SET);

    // Write the header record to the hashFile
    fwrite(pHashHeader, pHashHeader->iRecSize, 1L, hashFile.pFile);

    // Close the file
    fclose(hashFile.pFile);
    return RC_OK;
}

/*********************** hashOpen ****************************************
Purpose:
    Open a hash file for binary input and output, if it exists return an
    error. Returns the hash header record being found.
Parameters:
    char            szFileNm[ ]     Name for the file being opened
    hashFile        *pHashFile      Pointer for hashFile to be returned
Notes:
    Returns the hash header record through the parameter
Return Value:
    RC_FILE_NOT_FOUND       File not found
    RC_HEADER_NOT_FOUND     Header record not found
    RC_OK                   Executes successfully
*************************************************************************/
int hashOpen(char szFileNm[ ], HashFile *pHashFile) {
    int rc;

    // Set pHashFile to the file being read, if not found exit
    pHashFile->pFile = fopen(szFileNm, "rb+");
    if (pHashFile->pFile == NULL)
        return RC_FILE_NOT_FOUND;

    // Read the header record to get the starting point of the HashFile
    // If not found return ERROR
    rc = fread(&pHashFile->hashHeader, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
    if (rc != 1)
        return RC_HEADER_NOT_FOUND;

    return RC_OK;
}

/*********************** readRec *****************************************
Purpose:
    Read a record located at a particular RBA (relative byte address) and
    return the information from that record in the Hash File.
Parameters:
    HashFile        *pHashFile      pointer to the hash file where the
                                    record may be
    i               iRBN            RBN (relative byte number) where the
                                    record is located
    void            *pRecord        record being read
Notes:
    Values from the record are returned the parameter pRecord
Return Value:
    RC_LOC_NOT_FOUND    record location not found
    RC_OK               executes successfully
*************************************************************************/
int readRec(HashFile *pHashFile, int iRBN, void *pRecord) {
    long lRBA;
    int rcFread;

    // Determine the RBA by multiplying the RBN by the size of the records
    lRBA = iRBN * pHashFile->hashHeader.iRecSize;

    // Set the current position int the file to the correct RBA
    fseek(pHashFile->pFile, lRBA, SEEK_SET);

    // Read the record located at that position
    // If not found return ERROR
    rcFread = fread(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
    if (rcFread != 1)
        return RC_LOC_NOT_FOUND;

    return RC_OK;
}

/*********************** writeRec ****************************************
Purpose:
    Write a record to a particular RBA (relative byte address) in the
    Hash File.
Parameters:
    HashFile        *pHashFile      pointer to the hash file where the
                                    record will be written to
    i               iRBN            RBN (relative byte number) where the
                                    record will be written to
    void            *pRecord        record being written
Notes:
    Values that are being written are returned through the parameter
    pRecord
Return Value:
    RC_LOC_NOT_WRITTEN      record not written
    RC_OK                   executes successfully
*************************************************************************/
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord) {
    long lRBA;
    int rcFwrite;

    // Determine the RBA by multiplying the RBN by the size of the records
    lRBA = iRBN * pHashFile->hashHeader.iRecSize;

    // Set the current position int the file to the correct RBA
    fseek(pHashFile->pFile, lRBA, SEEK_SET);

    // Write the record to that position in the file
    // If the record was not written return ERROR
    rcFwrite = fwrite(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
    if (rcFwrite != 1)
        return RC_LOC_NOT_WRITTEN;

    return RC_OK;
}

/*********************** movieInsert *************************************
Purpose:
    Insert a movie to a location within a file, to be used in conjunction
    with lower level functions, readRec and writeRec to deal with the
    binary read and write.
Parameters:
    HashFile        *pHashFile      pointer to the file being inserted in
    Movie           *pMovie         pointer to the movie being inserted
Notes:
    Values from pMovie will be returned through the parameter pMovie.
Return Value:
    RC_OK               executes successfully
    RC_REC_EXISTS       record already exists
    RC_SYNONYM          there is a record already at the RBN
*************************************************************************/
int movieInsert(HashFile *pHashFile, Movie *pMovie) {
    int iRBN;
    int rcReadRec;
    int rcWriteRec;
    Movie pMovie2;

    // Determine the RBN by using the hash function (szMovieId is the key)
    iRBN = hash(pMovie->szMovieId, pHashFile->hashHeader.iNumPrimary);

    // Copy the information from the movie into a temp struct
    pMovie2 = *pMovie;

    // Read the record at that location using readRec
    // Will return the movie found through the parameter pMovie
    rcReadRec = readRec(pHashFile, iRBN, pMovie);

    // If there was no record found at that location
    // Write the record to it using writeRec
    // If not written return an ERROR
    if (rcReadRec == RC_LOC_NOT_FOUND) {
        pMovie->iNextChain = 0;
        rcWriteRec = writeRec(pHashFile, iRBN, pMovie);
        return RC_OK;
    } else if (pMovie->szMovieId[0] == '\0') {
        // If the location is filled with 0 bytes, write to the location using original info
        pMovie = &pMovie2;
        rcWriteRec = writeRec(pHashFile, iRBN, pMovie);
        return RC_OK;
    } else if (rcReadRec == RC_OK && strcmp(pMovie->szMovieId, pMovie2.szMovieId) == 0) {
        // If a record exists at that location, and the szMovieId matches that record already exists
        return RC_REC_EXISTS;
    }

    // Otherwise, a verified record already exists at that location (chaining needed)
    return RC_SYNONYM;
}

/*********************** movieRead **************************************
Purpose:
    Read a movie from a location within a file, to be used in conjunction
    with lower level function, readRec, to deal with the binary read.
Parameters:
    hashFile        *pHashFile      pointer to the file being read
    Movie           *pMovie         pointer to the movie values being read
Notes:
    Values from pMovie are being returned through the parameter pMovie.
Return Value:
    RC_REC_NOT_FOUND        record not found
    RC_OK                   executes successfully
*************************************************************************/
int movieRead(HashFile *pHashFile, Movie *pMovie) {
    int iRBN;
    int rcReadRec;

    // Determine the RBN by using the hash function (szMovieId is the key)
    iRBN = hash(pMovie->szMovieId, pHashFile->hashHeader.iNumPrimary);

    // Read the record at that RBN
    rcReadRec = readRec(pHashFile, iRBN, pMovie);

    // If the record is filled with 0 bytes, the record was not found
    if (pMovie->szMovieId[0] == '\0')
        return RC_REC_NOT_FOUND;

    // If no record was found, the record does not exist
    if (rcReadRec == RC_LOC_NOT_FOUND)
        return RC_REC_NOT_FOUND;

    return RC_OK;
}
