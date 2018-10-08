/**********************************************************************
    cs3743p2.h
    Copyright 2018 Larry Clark, this code may not be copied to any other website
Purpose:
    Defines constants for
        return codes
        maximum record size
    Defines typedefs for
        HashHeader - describes the format of the hash file
        HashFile - contains the file pointer and hashHeader
        Movie - describes the contents of a movie record
    Prototypes
        functions defined in the driver
        functions the students must code
Notes:
    
**********************************************************************/

#include <stdio.h>
#define TRUE                1
#define FALSE               0
#define RC_OK               0
#define RC_FILE_EXISTS      1   // file already exists
#define RC_REC_EXISTS       2   // record already exists
#define RC_REC_NOT_FOUND    3   // record not found
#define RC_FILE_NOT_FOUND   4   // file not found
#define RC_HEADER_NOT_FOUND 5   // Header record not found
#define RC_BAD_REC_SIZE     6   // invalid record size in info record
#define RC_LOC_NOT_FOUND    7   // Location not found for read
#define RC_LOC_NOT_WRITTEN  8   // Location not written for write
#define RC_SYNONYM          9   // Synonym exists at the specified location
#define RC_NOT_IMPLEMENTED  10    // Not implemented
#define RC_INVALID_PARM     99

#define MAX_REC_SIZE        500

// HashHeader structure for record at RBN 0. 
// Notes:
//     - do NOT write the sizeof this record.  Instead,
//       use the iRecSize.
//     - it contains filler on the end to make certain
//     - it is as big as a hash record.
typedef struct
{
    int  iNumPrimary;       // Number of records defined for Primary 
                            // area.  (This isn't the number of
                            // records currently in the Primary Area.)
    int  iHighOverflowRBN;  // Highest record used in the overflow area
    int  iRecSize;          // Size in bytes for each record in the
                            // hash file
    char sbFiller[MAX_REC_SIZE];     // This should be set to zeros.  It is 
                            // here to make certain the Info record is 
                            // at least as big as the data.  When writing,
                            // use iRecsize not sizeof(HashHeader)
} HashHeader;
// HashFile structure containing a FILE pointer and HashHeader
// record for the hash file.
// Notes:
//     - 
typedef struct
{
    FILE *pFile;            // FILE pointer to the hash file
    HashHeader hashHeader;  // the header record contents for a hash file
} HashFile;

// Movie is the structure that contains movie data and a next chain RBN.
// In program #2, iNextChain is used for collision chains.
typedef struct 
{
    int  iNextChain;        // RBN of next synonym
    char szMovieId[12];     // Individual Movie Id 
    char szTitle[32];       // Title
    char szGenre[12];       // Movie genre (e.g., COMEDY, HORROR)
    char szRating[5];       // Rating (e.g., G, PG, PG13, R, X
    int  iMinutes;          // Length of movie in minutes
} Movie;

// Driver functions that the student can use
int printAll(char szFileNm[]);
int hash(char szKey[], int iNumPrimary);
void printMovie(Movie *pMovie);
void errExit(const char szFmt[], ... );


// Functions written by the student
int hashCreate(char szFileNm[], HashHeader *pHashHeader);
int hashOpen(char szFileNm[], HashFile *pHashFile);
int readRec(HashFile *pHashFile, int iRBN, void *pRecord);
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord);
int movieInsert(HashFile *pHashFile, Movie *pMovie);
int movieRead(HashFile *pHashFile, Movie *pMovie, int *piRBN);
int movieUpdate(HashFile *pHashFile, Movie *pMovie);
int movieDelete(HashFile *pHashFile, Movie *pMovie); 
