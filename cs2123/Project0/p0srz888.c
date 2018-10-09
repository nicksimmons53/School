/******************************************************************************
cs2123p0.c by your Nicholas Simmons
Purpose:
    This program reads courses and their attributes.  It displays that information.
Command Parameters:
    n/a
Input:
    The standard input file stream contains two types of records (terminated
    by EOF).  COURSE records are followed by zero to many ATTR records 
    (terminated by either EOF or another COURSE record).
    COURSE szCourseId szName
      8s           7s        20s (but the name may have blanks)
    ATTR    szAttrType  szAttrValue
      8s          10s        12s
Results:
    For each course, it prints the course ID, name, and the attributes.
Returns:
    0 - normal
    1 - too many courses
    2 - too many attributes
    3 - bad input data
Notes:
    1. This program only allows for 30 courses.
    2. A course may have multiple occurrences of the same attribute type.  For example,
       a course may have many PREREQ.
    3. The number of attributes for a course must not exceed 12.
*******************************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

// Include files

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cs2123p0.h"

int main(int argc, char *argv[])
{
    Course courseM[MAX_COURSE];     // course array
    int iNumberOfCourses = 0;       // number of courses in courseM

    iNumberOfCourses = getCourseData(courseM);

    printCourseData(courseM, iNumberOfCourses);

    return 0;
}

/**********************getCourseData*********************************************************
int getCourseData(Course courseM[ ])

Purpose:
    Opens the course file to read the data until szRecordType[ATTR] or EOF is detected. Will
    return the index of the next course in the array. 

Parameters:
    I Course courseM[ ]     Course array that is being read.

Notes:
    I MAX_COURSE    Maximum number of courses allowed.

    The program will give an error for a max number of courses exceeded and also for bad 
    input. 

Return Value:
    i + 1       Index of the next course in the array.
*********************************************************************************************/
int getCourseData(Course courseM[])
{
    FILE *pfileCourse;                      // input stream file
    char szInputBuffer[MAX_LINE_SIZE + 1];  // input buffer for fgets

    int iNumAttr = 0;                       // Number of attributes for the current course
    char szRecordType[11];                  // record type of either COURSE or ATTR
    int i = -1;                             // current course subscript. -1 indicates 
                                            // not on a course yet
    int iScanfCnt;                          // scanf returns the number of successful inputs
    char *pszRemainingTxt;                  // After grabbing a token, this is the next
                                            // position.  This will be after the delimiter
                                            // unless the string was terminated by a zero
                                            // byte, then it will be on the zero byte.
    pfileCourse = stdin;

    // read data input lines of text until EOF.  fgets returns NULL at EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pfileCourse) != NULL)
    {
        // if the line is just a line feed, skip it.
        if (szInputBuffer[0] == '\n')
            continue;
    
        // print the input buffer as is (it also has a linefeed)
        // printf("%s", szInputBuffer);

        // get the COURSE or ATTR command
        pszRemainingTxt = getToken(szInputBuffer, szRecordType, sizeof(szRecordType)-1);

        // see if getting a course or a attribute
        if (strcmp(szRecordType, "COURSE") == 0)
        {
            i++;
            // see if we have too many courses to fit in the array
            if (i >= MAX_COURSE)
                ErrExit(ERR_TOO_MANY_COURSE
                    , "Invalid input file, max courses is %d"
                    , MAX_COURSE);

            iNumAttr = 0;   // since we have a new course, reset its number of attributes
            courseM[i].iNumberOfAttrs = iNumAttr;
            iScanfCnt = sscanf(pszRemainingTxt, "%7s %20[^\n]\n"
                , courseM[i].szCourseId
                , courseM[i].szCourseName);
            
            // Check for bad input.  scanf returns the number of valid conversions
            if (iScanfCnt < 2)
            {
                WARNING("Expected ID and name, received %d successful values\nInput text: %s\n"
                    , iScanfCnt, szInputBuffer);
                continue;
            }
        }
        else if (strcmp(szRecordType, "ATTR") == 0)
        {
            // what if we haven't received a COURSE record yet
            if (i < 0)
                ErrExit(ERR_BAD_INPUT
                    , "ATTR record without COURSE\nInput Text: %s\n"
                    , szInputBuffer);

            // what if we have too many attributes 
            if (iNumAttr >= MAX_ATTR)
                ErrExit(ERR_BAD_INPUT
                    , "Too many attributes for Course ID %s, only %d attributes allowed"
                    , courseM[i].szCourseId
                    , MAX_ATTR);

            iScanfCnt = sscanf(pszRemainingTxt, "%10s %12s"
                , courseM[i].attrM[iNumAttr].szAttrType
                , courseM[i].attrM[iNumAttr].szAttrValue);

            // Check for bad input.  scanf returns the number of valid conversions
            if (iScanfCnt < 2)
            {
                WARNING(
                    "Expected attribute type and value, received %d successful values"
                    , iScanfCnt);
                continue;
            }
            iNumAttr++;
            courseM[i].iNumberOfAttrs = iNumAttr;
        }
        else
        {
            WARNING("Bad Command in input, found '%s'"
                , szRecordType);
            continue;
        }
    }
    return i + 1;
}

/**************************************** printCourseData ***********************************
void printCourseData(Course courseM[ ], int iNumCourse)

Purpose: 
    Prints the course data for the entire courseM array into segregated columns and rows. 

Parameters:
    I   courseM[ ]      Array of all of the courses and information
    I   iNumCourse      Number of all of the courses

Notes:

Return Value:

Advice:  Print a course's info on a line followed on subsequent lines
         with indented attributes

Example Output:
                ID         Course Name
                                        Attr       Value
                CS1083     Intro I
                                        LANG       JAVA
                                        PROF       ROBBINS
                                        PROF       HOLLAND
                                        PROF       SHERETTE
                                        PROF       ROBINSON
                                        PROF       ZHU
                                        RECIT      N
                                        DEPT       CS
                MAT2233    Discrete Math
                                        PREREQ     MAT1214
                                        PREREQ     CS1713
                                        PROF       SHERETTE
                                        PROF       TANG
                                        RECIT      N
                                        DEPT       MAT
                MAT3333    Math Found
                                        PREREQ     MAT1223
                                        PREREQ     CS1713
                                        PROF       SHERETTE
                                        PROF       TANG
                                        RECIT      N
                                        DEPT       MAT
                CS1713     Intro II
                                        PREREQ     CS1083
                                        LANG       C
                                        PROF       CLARK
                                        PROF       HOLLAND
                                        PROF       SHERETTE
                                        PROF       ROBINSON
                                        PROF       ZHU
                                        RECIT      Y
                                        DEPT       CS
                CS2123     Data Structures
                                        PREREQ     CS1713
                                        LANG       C
                                        PROF       CLARK
                                        PROF       SHERETTE
                                        PROF       ROBINSON
                                        PROF       KORKMAZ
                                        PROF       TOSUN
                                        RECIT      Y
                                        DEPT       CS


********************************************************************************************/
void printCourseData(Course courseM[], int iNumCourse)
{
    int i;
    int j;

    // Print a heading for the list of courses and attributes
    printf(" %-7s %-20s\n", "Id", "Course Name");
    printf(" \t\t\t %-10s %-12s\n", "Attr", "Value");
    
    for (i = 0; i < iNumCourse; i++)
    {
        // Print a course Id and Name
        printf(" %-7s %-20s\n", courseM[i].szCourseId, courseM[i].szCourseName);
        
        //  Print each attribute 
        for (j = 0; j < courseM[i].iNumberOfAttrs; j++)
        {
            printf(" \t\t\t %-10s %-20s\n"
                , courseM[i].attrM[j].szAttrType
                , courseM[i].attrM[j].szAttrValue);
        }
    }
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also 
    returns the position in the text after that token.
Parameters:
    I   char *pszInputTxt       input buffer to be parsed
    O   char szToken[]          Returned token.
    I   int iTokenSize          The size of the token variable.  This is used
                                to prevent overwriting memory.  The size 
                                should be the memory size minus 1 (for
                                the zero byte).
Returns:
    Functionally: 
         Pointer to the next character following the delimiter after the token.  
         NULL - no token found.
    szToken parm - the returned token.  If not found, it will be an 
         empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
    - If the input buffer pointer is NULL, the function terminates with
      an algorithm error.
**************************************************************************/
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n\r";        // delimiters
    szToken[0] = '\0';

    // check for NULL pointer 
    if (pszInputTxt == NULL)
        ErrExit(ERR_ALGORITHM
            , "getToken passed a NULL pointer"); 

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else
        return pszInputTxt + 1;
}
/******************** ErrExit **************************************
  void ErrExit(int iexitRC, char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program with the specified exit return code.
Parameters:
    I   int iexitRC             Exit return code for the program
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Returns:
    Returns a program exit return code:  the value of iexitRC.
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
**************************************************************************/
void ErrExit(int iexitRC, char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n\tEncountered in file %s\n", __FILE__);  // this 2nd arg is filled in by
                                // the pre-compiler
    exit(iexitRC);
}
