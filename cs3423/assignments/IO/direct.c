/**********************************************************************
direct.c by Larry Clark
Purpose:
    This program demonstrates the use of a direct access file
    containing binary data.
Command Parameters:
    direct -i inputTextFileName -o directFile -n 
            reads the stream input file to create/manipulate 
            a binary file
Input:
    Input Text File is a stream input text file containing:
        cCommand lRBA additonal values depending on the command
           1c      d
        Commands:
            R - reads (and prints) the record at the specified location
            W - writes the record at the specified location.  The new
                information is on the rest of the text line in the input:
                   szEmpId dHourlyRate iExemptCnt cMaritalStatus dExtraWith szFullName
                   %6s %lf %d %c %lf %40[^\n]
        
Results:
    The W command causes this code to write records to the file; therefore, 
    populating it.
    
    During processing, it shows what it is writing as printable text.
    
    After all commands have been processed, it prints all the records 
    including "empty" ones.
Returns:
    0      normal
    1      invalid command line syntax
    2      show usage only
   99      error during processing, see stderr for more information
Notes:
   direct -?  will provide the usage information.  In some shells,
                  you will have to type direct -\?
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include "direct.h"

/* files */
FILE *pFileDirect;                  // Binary Direct file 

FILE *pFileInputTxt;                // Input text file

/* prototypes */
void processCommandSwitches(int argc, char *argv[], char **ppszDirectFileName
    , char **ppszInputTxtFileName);

void printFile(char *pszDirectFileName, char szTitle[]);
void processInputCommands(char *pszInputTxtFileName, char *pszNewDirectFileName);

int main(int argc, char *argv[])
{
    char            *pszDirectFileName = NULL;
    char            *pszInputTxtFileName = NULL;
    int             rc;
    int             iCommandType;                   // type of command

    /* get the filenames and comand type from the command switches */
    processCommandSwitches(argc, argv, &pszDirectFileName
        , &pszInputTxtFileName);

    processInputCommands(pszInputTxtFileName, pszDirectFileName);
    return 0;
}


/******************** processInputCommands **********************************
    void processInputCommands(char *pszInputTxtFileName, char *pszDirectFileName)
Purpose:
    Reads a input txt file containing commands and employee information and writes 
    a direct file.
Parameters:
    I   char *pszInputTxtFileName       input txt file name
    I   char *pszDirectFileName         direct file name
Notes:
    1. This function will open and close the specified files.
**************************************************************************/
void processInputCommands(char *pszInputTxtFileName, char *pszDirectFileName)
{
    Employee employee;                                // employee data
    char szInputBuffer[100];
    char cCommand;
    long lRBA;
    long lRecNum;
    char szRemaining[100];
    int iScanfCnt;
    int iWriteNew;                                  // 1 - wrote 1 record 
                                                    // 0 - error
    int rcFseek;
    int rc;

    // open the txt file for read
    pFileInputTxt = fopen(pszInputTxtFileName, "r");
    if (pFileInputTxt == NULL)
        errExit(ERR_INPUT_TXT_FILENAME, pszInputTxtFileName);

    // open the New Direct data file for write binary
    // If it already exists, we simply update it
    pFileDirect = fopen(pszDirectFileName, "wb+");
    if (pFileDirect == NULL)
        errExit(ERR_UPDATE_FILENAME, pszDirectFileName);

    /* get commands until EOF
    ** fgets returns null when EOF is reached.
    */

    while (fgets(szInputBuffer, 100, pFileInputTxt) != NULL)
    {
        if (szInputBuffer[0] == '\n')
            continue;
        printf("> %s", szInputBuffer);
        iScanfCnt = sscanf(szInputBuffer, "%c %ld %99[^\n]\n"
            , &cCommand
            , &lRecNum
            , szRemaining);
        if (iScanfCnt < 2)
        {
            printf("Error: Expected command and RBA, found: %s\n"
                , szInputBuffer);
            continue;
        }
        switch(cCommand)
        {
            case 'W':
                iScanfCnt = sscanf(szRemaining, "%6s %lf %d %c %lf %40[^\n]\n"
                    , employee.szEmployeeId
                    , &employee.dHourlyRate
                    , &employee.w4.iExemptionCnt
                    , &employee.w4.cFillingStatus
                    , &employee.w4.dWithholdExtra
                    , employee.szFullName);

                // Check for bad input.  scanf returns the number of valid conversions
                if (iScanfCnt < 6)
                    errExit(ERR_INVALID_EMPLOYEE_DATA, szInputBuffer);
                lRBA = lRecNum*sizeof(Employee);
                rcFseek = fseek(pFileDirect, lRBA, SEEK_SET);
                assert(rcFseek == 0);
                // write it to the direct file
                iWriteNew = fwrite(&employee
                    , sizeof(Employee)
                    , 1L
                    , pFileDirect);
                assert(iWriteNew == 1); 
                break;
            case 'R':
                lRBA = lRecNum*sizeof(Employee);
                rcFseek = fseek(pFileDirect, lRBA, SEEK_SET);
                assert(rcFseek == 0);
                // print the information at the RBA
                rc = fread(&employee, sizeof(Employee), 1L, pFileDirect);
                if (rc == 1)
                    printf("%-7s  %8.2lf  %5d      %c    %8.2lf     %-40s\n"
                        , employee.szEmployeeId
                        , employee.dHourlyRate
                        , employee.w4.iExemptionCnt
                        , employee.w4.cFillingStatus
                        , employee.w4.dWithholdExtra
                        , employee.szFullName);
                else 
                    printf("Record number %ld not found for RBA %ld\n", lRecNum, lRBA);    
                break;
            default:
                printf("unknown command\n");   
        }
    }
    // close the files
    fclose(pFileInputTxt);
    fclose(pFileDirect);
}
/******************** printFile ********************************************
    void printFile(char *pszDirectFileName, char szTitle[])
Purpose:
    Prints the contents of a binary master file.
Parameters:
    I   char *pszNewDirectFileName      master file name
    I   char szTitle[]                  title to print with file contents
Notes:
    1. This function will open and close the specified file.
**************************************************************************/
void printFile(char *pszDirectFileName, char szTitle[])
{
    FILE *pfileEmployee;
    Employee employee;
    printf("%s for file %s\n%-7s  %8s  %6s  %6s  %10s  %-40s\n"
        , szTitle
        , pszDirectFileName
        , "Employee"
        , "Pay Rate"
        , "Exempt"
        , "Status"
        , "With Extra"
        , "Full Name");
    // open the file
    pfileEmployee = fopen(pszDirectFileName, "rb");
    if (pfileEmployee == NULL)
        errExit(ERR_MASTER_FILENAME, pszDirectFileName);
    // read the Employee data until EOF
    while (fread(&employee, sizeof(Employee), 1L, pfileEmployee) == 1)
    {
        printf("%-7s  %8.2lf  %5d      %c    %8.2lf     %-40s\n"
            , employee.szEmployeeId
            , employee.dHourlyRate
            , employee.w4.iExemptionCnt
            , employee.w4.cFillingStatus
            , employee.w4.dWithholdExtra
            , employee.szFullName);
    }
    fclose(pfileEmployee);
}
/******************** processCommandSwitches *****************************
void processCommandSwitches(int argc, char *argv[], char **ppszDirectFileName
    , char **ppszInputTxtFileName);
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszDirectFileName       Direct file name 
    O   char **ppszInputTxtFileName     Input Text File containing simple employee data.
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszDirectFileName
    , char **ppszInputTxtFileName)
{
    int i;

    // If there aren't any arguments, show the usage
    if (argc <= 1)
        exitUsage(0, "No arguments", "");

    // Examine each of the command arguments other than the name of the program.
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
             case '?':
                exitUsage(USAGE_ONLY, "", "");
                break;
            case 'o':                   // Direct File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                // check for too long of a file anme
                else
                    *ppszDirectFileName = argv[i];
                break;
            case 'i':                   // Input Txt File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                // check for too long of a file anme
                else
                    *ppszInputTxtFileName = argv[i];
                break;
            default:
                exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
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
    Exits the program with a return code of 99.
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
/******************** exitUsage *****************************
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg > 1)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
        , iArg
        , pszMessage
        , pszDiagnosticInfo);
    fprintf(stderr, "direct -i inputTxtFile -o directFile\n");
    if (iArg >= 0)
        exit(ERR_COMMAND_LINE_SYNTAX);
    else
        exit(USAGE_ONLY);
}
