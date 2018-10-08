/**********************************************************************
empMerge.c by Larry Clark
Purpose:
    This program demonstrates how to merge 2 files creating a new 
    master file.  
    It also shows how to create a master file from a stream input file.
Command Parameters:
    empMerge -m masterFileName -u updateFileName -n newMasterFileName
            causes empMerge to merge the -m and -u files creating the -n
    empMerge -i inputFileName -n newMasterFileName 
            reads the stream input file to create a binary file
    empMerge -p masterFileName
            simply prints the contents of the specified file
Input:
    For -m: (merge)
        master file -  binary file containing Employee records
        update file -  binary file containing updated Employee records
    For -i: (input text to binary)
        master file -  binary file containing Employee records
        input txt file - stream input text file containing employee data
    For -p: (print contents of binary)
        master file -  binary file containing Employee records
Results:
    For -m: (merge)
        1. prints the contents of the specified binary master file
        2. prints the contents of the specified binary update file
        3. merges the master and update file writing a new master file.
        4. prints the contents of the specified binary new master file
    For -i: (input text to binary)
        1. creates a new master from a stream input text file.
        2. prints the contents of the specified binary new master file
    For -p: (print contents of binary)
        1. prints the contents of the specified binary master file 
Returns:
    0      normal
    1      invalid command line syntax
    2      show usage only
   99      error during processing, see stderr for more information
Notes:
   empMerge -?  will provide the usage information.  In some shells,
                  you will have to type empMerge -\?
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include "empMerge.h"

/* files */
FILE *pfileMaster;                  // Binary Master file 
FILE *pfileUpdate;                  // Binary Updates file 
FILE *pfileNewMaster;               // Binary New Master file

FILE *pfileInputTxt;                // Input text file

/* prototypes */
void processCommandSwitches(int argc, char *argv[], char **ppszMasterFileName
    , char **ppszUpdateFileName, char ** ppszNewMasterFileName
    , char **ppszInputTxtFileName, int *piComandType);
void merge(char *pszMasterFileName, char *pszUpdateFileName
    , char *pszNewMasterFileName);
void printFile(char *pszMasterFileName, char szTitle[]);
void readTxtWriteMaster(char *pszInputTxtFileName, char *pszNewMasterFileName);

int main(int argc, char *argv[])
{
    char            *pszMasterFileName = NULL;
    char            *pszUpdateFileName = NULL;
    char            *pszNewMasterFileName = NULL;
    char            *pszInputTxtFileName = NULL;
    int             rc;
    int             iCommandType;                   // type of command

    /* get the filenames and comand type from the command switches */
    processCommandSwitches(argc, argv, &pszMasterFileName, &pszUpdateFileName
        , &pszNewMasterFileName, &pszInputTxtFileName, &iCommandType);

    // which type of command are we doing? 

    switch (iCommandType)
    {
        case COMMAND_MERGE:     // merge 2 files, producing new master
            if (pszMasterFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-m");
            if (pszUpdateFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-u");
            if (pszNewMasterFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-n");
            printFile(pszMasterFileName, "Master File");
            printFile(pszUpdateFileName, "Updates File");
            merge(pszMasterFileName, pszUpdateFileName, pszNewMasterFileName);
            printFile(pszNewMasterFileName, "Results for Merge");
            break;

        case COMMAND_CREATE:    // create a master from txt
            if (pszNewMasterFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-n");
            if (pszInputTxtFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-i");
            readTxtWriteMaster(pszInputTxtFileName, pszNewMasterFileName);
            printFile(pszNewMasterFileName, "Results for Create");
            break;

        case COMMAND_PRINT:     // only print a data file
            if (pszMasterFileName == NULL)
                errExit(ERR_MISSING_SWITCH, "-p");
            printFile(pszMasterFileName, "Results for -p Print");
            break;
    }
    return 0;
}

/******************** merge ***********************************************
void merge(char *pszMasterFileName, char *pszUpdateFileName
, char *pszNewMasterFileName)
Purpose:
    merges the Master File and Update File producing a New Master File.
Parameters:
    I   char *pszMasterFileName         current mast file name
    I   char *pszUpdateFileName         update file name
    I   char *pszNewMasterFileName      new master file name
Notes:
    1. This function will open and close the specified files.
**************************************************************************/
void merge(char *pszMasterFileName, char *pszUpdateFileName
    , char *pszNewMasterFileName)
{
    Employee employeeMaster;                              // record from Master
    Employee employeeUpdate;                              // record from Update
    int iGotMaster;                                     // 1 - got a Master rec
                                                        // 0 - no data
    int iGotUpdate;                                     // 1 - got a Update rec
                                                        // 0 - no data
    int iWriteNew;                                      // 1 - wrote 1 record 
                                                        // 0 - error

    // open master
    pfileMaster = fopen(pszMasterFileName, "rb");
    if (pfileMaster == NULL)
        errExit(ERR_MASTER_FILENAME, pszMasterFileName);

    // open the Update data file
    pfileUpdate = fopen(pszUpdateFileName, "rb");
    if (pfileUpdate == NULL)
        errExit(ERR_UPDATE_FILENAME, pszUpdateFileName);

    // open the new master data file
    pfileNewMaster = fopen(pszNewMasterFileName, "wb");
    if (pfileNewMaster == NULL)
        errExit(ERR_NEW_MASTER_FILENAME, pszNewMasterFileName);

    // read the first record from each file
    iGotMaster = fread(&employeeMaster, sizeof(Employee), 1L, pfileMaster);
    iGotUpdate = fread(&employeeUpdate, sizeof(Employee), 1L, pfileUpdate);

    // continue in loop until one of the files reaches EOF

    while (iGotMaster == 1 && iGotUpdate == 1)
    {
        // Compare the keys to see which to write to the new master
        if (strcmp(employeeMaster.szEmployeeId, employeeUpdate.szEmployeeId) < 0)
        {   // Master's key is lower so write New Master from it
            iWriteNew = fwrite(&employeeMaster
                , sizeof(Employee)
                , 1L
                , pfileNewMaster);
            assert(iWriteNew == 1);
            iGotMaster = fread(&employeeMaster, sizeof(Employee), 1L, pfileMaster);
        }
        else if (strcmp(employeeMaster.szEmployeeId, employeeUpdate.szEmployeeId) > 0)
        {   // Update's key is lower so write New Master from it
            iWriteNew = fwrite(&employeeUpdate
                , sizeof(Employee)
                , 1L
                , pfileNewMaster);
            assert(iWriteNew == 1);
            iGotUpdate = fread(&employeeUpdate, sizeof(Employee), 1L, pfileUpdate);
        }
        else
        {   // same key value, so write from Update and read both files
            iWriteNew = fwrite(&employeeUpdate
                , sizeof(Employee)
                , 1L
                , pfileNewMaster);
            assert(iWriteNew == 1);
            iGotUpdate = fread(&employeeUpdate, sizeof(Employee), 1L, pfileUpdate);
            iGotMaster = fread(&employeeMaster, sizeof(Employee), 1L, pfileMaster);
        }
    }

    // finish off Master until eof
    while (iGotMaster == 1)
    {
        iWriteNew = fwrite(&employeeMaster
            , sizeof(Employee)
            , 1L
            , pfileNewMaster);
        assert(iWriteNew == 1);
        iGotMaster = fread(&employeeMaster, sizeof(Employee), 1L, pfileMaster);
    }

    // finish off Update file until eof
    while (iGotUpdate == 1)
    {
        iWriteNew = fwrite(&employeeUpdate
            , sizeof(Employee)
            , 1L
            , pfileNewMaster);
        assert(iWriteNew == 1);
        iGotUpdate = fread(&employeeUpdate, sizeof(Employee), 1L, pfileUpdate);
    }
    // close the files
    fclose(pfileMaster);
    fclose(pfileUpdate);
    fclose(pfileNewMaster);
}

/******************** readTxtWriteMaster **********************************
    void readTxtWriteMaster(char *pszInputTxtFileName, char *pszNewMasterFileName)
Purpose:
    Reads a input txt file containing employee information and writes 
    a new master file.
Parameters:
    I   char *pszInputTxtFileName       input txt file name
    I   char *pszNewMasterFileName      new master file name
Notes:
    1. This function will open and close the specified files.
**************************************************************************/
void readTxtWriteMaster(char *pszInputTxtFileName, char *pszNewMasterFileName)
{
    Employee employee;                                // employee data
    char szInputBuffer[100];
    char *pszGetsResult;
    int iScanfCnt;
    int iWriteNew;                                  // 1 - wrote 1 record 
                                                    // 0 - error

    // open the txt file for read
    pfileInputTxt = fopen(pszInputTxtFileName, "r");
    if (pfileInputTxt == NULL)
        errExit(ERR_INPUT_TXT_FILENAME, pszInputTxtFileName);

    // open the New Master data file for write binary
    pfileNewMaster = fopen(pszNewMasterFileName, "wb");
    if (pfileNewMaster == NULL)
        errExit(ERR_UPDATE_FILENAME, pszNewMasterFileName);

    /* get employee data until EOF
    ** fgets returns null when EOF is reached.
    */

    pszGetsResult = fgets(szInputBuffer, 100, pfileInputTxt);
    while (pszGetsResult != NULL)
    {
        iScanfCnt = sscanf(szInputBuffer, "%6s %lf %d %c %lf %40[^\n]\n"
            , employee.szEmployeeId
            , &employee.dHourlyRate
            , &employee.w4.iExemptionCnt
            , &employee.w4.cFillingStatus
            , &employee.w4.dWithholdExtra
            , employee.szFullName);

        // Check for bad input.  scanf returns the number of valid conversions
        if (iScanfCnt < 6)
            errExit(ERR_INVALID_EMPLOYEE_DATA, szInputBuffer);

        // write it to the new master
        iWriteNew = fwrite(&employee
            , sizeof(Employee)
            , 1L
            , pfileNewMaster);
        assert(iWriteNew == 1);
        pszGetsResult = fgets(szInputBuffer, 100, pfileInputTxt);
    }
    // close the files
    fclose(pfileInputTxt);
    fclose(pfileNewMaster);
}
/******************** printFile ********************************************
    void printFile(char *pszMasterFileName, char szTitle[])
Purpose:
    Prints the contents of a binary master file.
Parameters:
    I   char *pszNewMasterFileName      master file name
    I   char szTitle[]                  title to print with file contents
Notes:
    1. This function will open and close the specified file.
**************************************************************************/
void printFile(char *pszMasterFileName, char szTitle[])
{
    FILE *pfileEmployee;
    Employee employee;
    printf("%s for file %s\n%-7s  %8s  %6s  %6s  %10s  %-40s\n"
        , szTitle
        , pszMasterFileName
        , "Employee"
        , "Pay Rate"
        , "Exempt"
        , "Status"
        , "With Extra"
        , "Full Name");
    // open the file
    pfileEmployee = fopen(pszMasterFileName, "rb");
    if (pfileEmployee == NULL)
        errExit(ERR_MASTER_FILENAME, pszMasterFileName);
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
void processCommandSwitches(int argc, char *argv[], char **ppszMasterFileName
, char **ppszUpdateFileName, char ** ppszNewMasterFileName
, char **ppszInputTxtFileName, int *piComandType);
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszMasterFileName       Master file name 
    O   char **ppszUpdateFileName       Update file name
    O   char **ppszNewMasterFileName    New Master file name
    O   char **ppszInputTxtFileName     Input Text File containing simple employee data.
    O   int  *piComandType              Set to one of the command types.
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszMasterFileName
    , char **ppszUpdateFileName, char ** ppszNewMasterFileName
    , char **ppszInputTxtFileName, int *piComandType)
{
    int i;
    *piComandType = COMMAND_MERGE;              // This is our default unless we
                                                // hit a -p or -i.
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
            case 'm':                   // Master File Name
            case 'p':
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                // check for too long of a file anme
                else
                    *ppszMasterFileName = argv[i];
                if (argv[i - 1][1] == 'p')
                    *piComandType = COMMAND_PRINT;
                break;
            case 'u':                   // Update File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else
                    *ppszUpdateFileName = argv[i];
                break;
            case '?':
                exitUsage(USAGE_ONLY, "", "");
                break;
            case 'n':                   // New Master File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                // check for too long of a file anme
                else
                    *ppszNewMasterFileName = argv[i];
                break;
            case 'i':                   // Input Txt File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                // check for too long of a file anme
                else
                    *ppszInputTxtFileName = argv[i];
                *piComandType = COMMAND_CREATE;
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
    fprintf(stderr, "empMerge -m masterFile -u updateFile -n newMasterFile\n");
    fprintf(stderr, "empMerge -i inputTxtFile -n newMasterFile\n");
    fprintf(stderr, "empMerge -p masterFile\n");
    if (iArg >= 0)
        exit(ERR_COMMAND_LINE_SYNTAX);
    else
        exit(USAGE_ONLY);
}
