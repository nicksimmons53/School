/**********************************************************************
direct.h
Purpose:
    This defines the typedefs, prototypes, and constants used
    for a program which does a simple file merge.
Notes:

**********************************************************************/
/* Employee typedef for one employee */
typedef struct
{
    int iExemptionCnt;          // Number of exemptions
    char cFillingStatus;        // M - Married, S - Single
                                // X - married but filling 
                                //     as single
    double dWithholdExtra;      // extra amount to withhold
} W4;

typedef struct
{
    char szEmployeeId[7];
    char szFullName[40];
    double dHourlyRate;
    W4 w4;
} Employee;

#define FALSE           0
#define TRUE            1

/* constants for the type of command */
#define COMMAND_MERGE   1
#define COMMAND_CREATE  2
#define COMMAND_PRINT   3

/*  Error Messages */
#define ERR_MISSING_SWITCH          "missing switch %s"
#define ERR_EXPECTED_SWITCH         "expected switch, found "
#define ERR_MISSING_ARGUMENT        "missing argument for %s"

#define ERR_MASTER_FILENAME         "invalid Master file name: '%s'"
#define ERR_INVALID_EMPLOYEE_DATA   "invalid employee data in file: '%s' "
#define ERR_NEW_MASTER_FILENAME     "invaid New Master file name: '%s'"
#define ERR_UPDATE_FILENAME         "invalid Update file name: '%s' "
#define ERR_INPUT_TXT_FILENAME      "invalid Input Txt file name: '%s'"

/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     1      // invalid command line syntax
#define USAGE_ONLY                  2      // show usage only
#define ERROR_PROCESSING            99     // error during processing

void errExit(const char szFmt[], ... );
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
