/******************************************************************************
cs2123p2Driver.c by Larry Clark
Purpose:
    This program reads queries, converts them from infix to postfix, and 
    executes the queries. It uses a stack for converting from infix to postfix
    and the execution of the postfix expression.
Command Parameters:
    p2 -c courseFile -q queryFile
Input:
    Course File:
        Input file stream contains two types of records (terminated
        by EOF).  COURSE records are followed by zero to many ATTR records
        (terminated by either EOF or another COURSE record).
        COURSE szCourseId szName
        8s           7s        20s (but the name may have blanks)
        ATTR    szAttrType  szAttrValue
        8s          10s        12s
    Query File:
        Input file stream contains infix expressions (one per input text line).
        Tokens in the expressions will be separated by one space.
        Some sample data:
            RECIT = N
            RECIT = Y
            PROF = CLARK
            PROF NEVER CLARK
            PROF ONLY CLARK
            PROF = CLARK AND RECIT = N
            PROF NEVER CLARK AND DEPT = CS
            RECIT = Y AND ( PROF = CLARK OR PROF = GIBSON )
            LANG ONLY C
            ( LANG ONLY C OR LANG ONLY JAVA ) AND PREREQ NEVER CS2123
            DEPT = CS AND RECIT = Y AND LANG = JAVA
            DEPT = CS AND ( RECIT = Y OR LANG = PYTHON ) AND PROF = CLARK
            DEPT = CS AND RECIT = Y OR LANG = PYTHON AND PROF = CLARK

Results:
    For each expression, print the original expression and its 
    corresponding postfix expression.
Returns:
    0 - normal
    901 - stack usage error (e.g., popping an empty stack)
    902 - PostfixOut overflow
    903 - algorithm error (see message for details)
Notes:
    1. This program uses an array to implement the stack.  It has a maximum of
       MAX_STACK_ELEM elements. 
    2. This program uses an PostfixOut array for the resulting postfix expression.
       It has a maximum of MAX_OUT_ITEM elements. 
    3. This program only allows for 30 courses.
    4. A course may have multiple occurrences of the same attribute type.  For example,
       a course may have many PREREQ.
    5. The number of attributes for a course must not exceed 12.
*******************************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p2.h"

// The following structure is used by the categorize function to help
// categorize tokens and provide the precedence.   
static struct
{
    char szSymbol[MAX_TOKEN + 1];   // token symbol such as "("
    int iCategory;                  // CAT_OPERATOR, CAT_OPERAND, CAT_LPAREN or CAT_RPAREN
    int iPrecedence;                // 0 - lowest precedence, 3 - highest precedence
} symbolDefM[] =
{
      {"(",      CAT_LPAREN,   0}
    , {")",      CAT_RPAREN,   0}
    , {"=",      CAT_OPERATOR, 2}
    , {"NEVER",  CAT_OPERATOR, 2}
    , {"ONLY",   CAT_OPERATOR, 2}
    , {"AND",    CAT_OPERATOR, 1}
    , {"OR",     CAT_OPERATOR, 1}
    , {"", 0, 0}
};

// Stack implementation using arrays.  You are not required to document these.
void push(Stack stack, Element value)
{
    if (stack->iCount >= MAX_STACK_ELEM)
        ErrExit(ERR_STACK_USAGE
        , "Attempt to PUSH more than %d values on the array stack"
        , MAX_STACK_ELEM);

    stack->stackElementM[stack->iCount] = value;
    stack->iCount++;
}
Element pop(Stack stack)
{
    if (isEmpty(stack))
        ErrExit(ERR_STACK_USAGE
        , "Attempt to POP an empty array stack");
    stack->iCount--;
    return stack->stackElementM[stack->iCount];

}
Element topElement(Stack stack)
{
    if (isEmpty(stack))
        ErrExit(ERR_STACK_USAGE
        , "Attempt to examine topElement of an empty array stack");
    return stack->stackElementM[stack->iCount-1];    // return the top
}
int isEmpty(Stack stack)
{
    return stack->iCount <= 0;
}

Stack newStack()
{
    Stack stack = (Stack) malloc(sizeof(StackImp));
    stack->iCount = 0;
    return stack;
}

void freeStack(Stack stack)
{
    free (stack);
}

// File pointers for the course file and the query file

FILE *pFileCourse;    // Used with the -c Course File
FILE *pFileQuery;       // Used with the -q Query File

// Main program for the driver

int main(int argc, char *argv[])
{
    Course courseM[MAX_COURSE];     // Course array
    int iNumberOfCourses = 0;       // Number of courses

    char *pszCourseFileNm = NULL;     // Pointer to an argv[] for course file name
    char *pszQueryFileNm = NULL;        // Pointer to an argv[] for query file name

    // get the file names from the command argument switches
    processCommandSwitches(argc, argv, &pszCourseFileNm, &pszQueryFileNm);

    // Open the Course File if a file name was provided

    if (pszCourseFileNm == NULL)
        exitUsage(USAGE_ERR, ERR_MISSING_SWITCH, "-c");

    pFileCourse = fopen(pszCourseFileNm, "r");
    if (pFileCourse == NULL)
        exitUsage(USAGE_ERR, "Invalid course file name, found "
        , pszCourseFileNm);

    // Open the Query file if a file name was provided

    if (pszQueryFileNm == NULL)
        exitUsage(USAGE_ERR, ERR_MISSING_SWITCH, "-q");

    pFileQuery = fopen(pszQueryFileNm, "r");
    if (pFileQuery == NULL)
        exitUsage(USAGE_ERR, "Invalid query file name, found "
        , pszQueryFileNm);

    // get and print the course data including attributes
    iNumberOfCourses = getCourseData(courseM);
    fclose(pFileCourse);
    printCourseData(courseM, iNumberOfCourses);

    // Read and process the queries
    readAndProcessQueries(courseM, iNumberOfCourses);
    fclose(pFileQuery);
    printf("\n");
    fclose(stdout);
    return 0;
}
/******************** readAndProcessQueries **************************************
   void readAndProcessQueries(Course courseM[], int iNumberOfCourses)
Purpose:
    Reads queries from the Query File, converts them to postfix (via 
    convertToPostfix), evaluates the postfix (via evaluatePostfix), and
    shows the courses that satisified the queries (via printQueryResult).   
Parameters:
    i Course courseM[]    array of courses and attributes   
    i int iNumCourse      number of courses in courseM 
Notes:
    - References the global:  pFileQuery    
**************************************************************************/
void readAndProcessQueries(Course courseM[], int iNumberOfCourses)
{
    // Allocate the memory for the postfix result
    PostfixOut postfixOut = malloc(sizeof(PostfixOutImp));
    // array (which corresponds to courseM via subscript) of booleans 
    // showing which courses satisfied a query
    QueryResult queryResultM[MAX_COURSE];
    char szInputBuffer[100];            // entire input line
    int rc;
    int iLineCount = 0;
    
    // read text lines containing expressions until EOF
   
    while (fgets(szInputBuffer, 100, pFileQuery) != NULL)
    {
        iLineCount++;

        // Ignore the line if it only contains a linefeed
        if (szInputBuffer[0] == '\n')
            continue;
        printf("Query # %d: %s", iLineCount, szInputBuffer);
        postfixOut->iPostfixOutCount = 0;   // reset out to empty

        // Invoke the student's convertToPostfix function
        rc = convertToPostfix(szInputBuffer, postfixOut);
        switch (rc)
        {
            case 0: // ok so print the postfix and evaluate it
                printPostfixOut(postfixOut);
                evaluatePostfix(postfixOut, courseM, iNumberOfCourses, queryResultM);
                printQueryResult(courseM, iNumberOfCourses, queryResultM);
                break;
            case WARN_MISSING_LPAREN:
                printf("\tWarning: missing left parenthesis\n");
                break;
            case WARN_MISSING_RPAREN:
                printf("\tWarning: missing right parenthesis\n");
                break;
            case WARN_MISSING_OPERATOR:
                printf("\tWarning: missing operator\n");
                break;
            case WARN_MISSING_OPERAND:
                printf("\tWarning: missing operand\n");
                break;
            default:
                printf("\t warning = %d\n", rc);
        }
        
    }
    printf("\n");
    free(postfixOut);
}
/******************** printQueryResult **************************************
void printQueryResult(Course courseM[], int iNumCourse, QueryResult resultM[])
Purpose:
    Prints the courses which have a corresponding element in the resultM array
    turned ON.   
Parameters:
    i Course courseM[]    array of courses and attributes   
    i int iNumCourse        number of courses in courseM 
    i QueryResult resultM[]   array (which corresponds to courseM bia subscript)
                              of booleans showing which courses satisfied a query
Notes:
    
**************************************************************************/
void printQueryResult(Course courseM[], int iNumCourse, QueryResult resultM[])
{
    int i;
    printf("\tQuery Result:\n");
    printf("\t%-7s  %-20s\n", "ID", "Course Name");
    // Loop through each course
    for (i = 0; i < iNumCourse; i++)
    {
        // Print courses having a corresponding result boolean which is TRUE
        if (resultM[i])
            printf("\t%-7s  %-20s\n", courseM[i].szCourseId
                , courseM[i].szCourseName);
    }
}
/******************** never **************************************
int never(Course *pCourse, Attr attr)
Purpose:
    Determines whether a course has a particular attribute (type and
    value).  If it doesn't, never returns TRUE; otherwise, it 
    returns FALSE.
Parameters:
    I Course *pCourse     One course structure which also
                             contains attributes of that course.
    I Attr    attr        The attribute that we don't want this 
                             course to have.
Notes:
    This function could be used by the function atLeastOne().
Return value:
    TRUE - course didn't have the specified attribute
    FALSE - course did have it
**************************************************************************/
int never(Course *pCourse, Attr attr)
{
    int i = 100;
    // Make certain the course information was passed to this function.
    if (pCourse == NULL)
        ErrExit(ERR_ALGORITHM
        , "function never() received a NULL pointer");

    // Loop through the attributes to see if the specified 
    // attribute type and value exists.  If so, never will be FALSE
    for (i = 0; i < (pCourse->iNumberOfAttrs); i++)
    {
        if (strcmp(pCourse->attrM[i].szAttrType, attr.szAttrType) == 0
            && strcmp(pCourse->attrM[i].szAttrValue, attr.szAttrValue) == 0)
            return FALSE;
    }
    return TRUE;
}
/******************** addPostfixOut **************************************
void addPostfixOut(PostfixOut postfixOut, Element element)
Purpose:
    Adds an element to postfixOut.  
Parameters:
    I/O PostfixOut postfixOut   Stores the postfix expression 
    I Element element           Element structure to be added to postfixOut. 
Returns:
    n/a 
Notes:
    - Since postfixOut uses an array, addPostfixOut does a boundary 
      check for overflow. 
**************************************************************************/
void addPostfixOut(PostfixOut postfixOut, Element element)
{
    if (postfixOut->iPostfixOutCount >= MAX_OUT_ITEM)
        ErrExit(ERR_OUT_OVERFLOW
        , "Overflow in the postfixOut array");
    postfixOut->postfixOutM[postfixOut->iPostfixOutCount] = element;
    postfixOut->iPostfixOutCount++;
}

/******************** printPostfixOut **************************************
void printPostfixOut(PostfixOut postfixOut)
Purpose:
    prints the contents of the postfixOutM array to stdout 
Parameters:
    I PostfixOut postfixOut     The postfx expression  
Returns:
    n/a 
Notes:
    - Prints 18 tokens from out per line
**************************************************************************/
void printPostfixOut(PostfixOut postfixOut)
{
    int i;
    if (postfixOut == NULL)
    {
        printf("*** ERROR PostfixOut is NULL\n");
        return;
    }
    printf("\t");
    // loop through each element in the out array
    for (i = 0; i < postfixOut->iPostfixOutCount; i++)
    {
        printf("%s ", postfixOut->postfixOutM[i].szToken);
        if ((i + 1) % 18 == 0)
            printf("\n\t");
    }
    printf("\n");
}

/******************** categorize **************************************
void categorize(Element *pElement)
Purpose:
    Categorizes a token providing its precedence (0 is lowest, higher 
    integers are a higher precedence) and category (operator, operand,
    left paren, right paren). 
    Since the category is an integer, it can be used in a switch statement.
Parameters:
    I/O Element *pElement       pointer to an element structure which
                                will be modified by this function
Returns:
    n/a 
Notes:
    - The input parameter, pElement must point to an existing Element
      structure.  Its attribute, szToken, must already be populated.
    - This function will populate the iCategory and
      iPrecedence values in the Element structure referenced by pElement.
    - Uses the symbolDefM array to help categorize tokens 
**************************************************************************/
void categorize(Element *pElement)
{
    int i;
    // loop through the symbolDefM array until an empty symbol value is found
    // marking the end of the symbolDefM array
    for (i = 0; symbolDefM[i].szSymbol[0] != '\0'; i++)
    {
        // does the element's token match the symbol in the symbolDefM array?
        if (strcmp(pElement->szToken, symbolDefM[i].szSymbol) == 0)
        {   // matched, so use its precedence and category
            pElement->iPrecedence = symbolDefM[i].iPrecedence;
            pElement->iCategory = symbolDefM[i].iCategory;
            return;
        }
    }
    // must be an operand
    pElement->iPrecedence = 0;
    pElement->iCategory = CAT_OPERAND;
}
/******************** getCourseData **************************************
int getCourseData(Course courseM[])
Purpose:
    Gets course data and their corresponding attributes (type and values).  
Parameters:
    O Course courseM[]      an array of courses to be returned  
Returns:
    Count of courses
Notes:
    - This function attempts to give warnings for minor errors found in 
      reading course data (e.g., bad command, bad format of data); 
      however, some problems cause termination (e.g., too many 
      attributes for a course).
    - It reads a course file using the global pFileCourse
        Contains two types of records (terminated
        by EOF).  COURSE records are followed by zero to many ATTR records
        (terminated by either EOF or another COURSE record).
        COURSE szCourseId szName
        8s           7s        20s (but the name may have blanks)
        ATTR    szAttrType  szAttrValue
        8s          10s        12s
**************************************************************************/

int getCourseData(Course courseM[])
{
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

    // read data input lines of text until EOF.  fgets returns NULL at EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pFileCourse) != NULL)
    {
        // if the line is just a line feed, skip it.
        if (szInputBuffer[0] == '\n')
            continue;

        // print the input buffer as is (it also has a linefeed)
        // printf("%s", szInputBuffer);

        // get the COURSE or ATTR command
        pszRemainingTxt = getToken(szInputBuffer, szRecordType, sizeof(szRecordType) - 1);

        // see if getting a course or a attribute
        if (strcmp(szRecordType, "COURSE") == 0)
        {
            i++;
            // see if we have too many courses to fit in the array
            if (i >= MAX_COURSE)
                ErrExit(ERR_TOO_MANY_COURSE
                , "Invalid input file, max courses is %d"
                , MAX_COURSE);

            iNumAttr = 0;   // since we have a new course, reset his/her number of attributes
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
                ErrExit(ERR_BAD_COURSE
                , "ATTR record without COURSE\nInput Text: %s\n"
                , szInputBuffer);

            // what if we have too many attributes 
            if (iNumAttr >= MAX_ATTR)
                ErrExit(ERR_BAD_COURSE
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
/******************** processCommandSwitches *****************************
    void processCommandSwitches(int argc, char *argv[], char **ppszCourseFileName
        , char **ppszQueryFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.  
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszCourseFileName     Course File Name to return
    O   char **ppszQueryFileName        Query File Name to return 
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszCourseFileName
    , char **ppszQueryFileName)
{
    int i;
    // Examine each of the command arguments other than the name of the program.
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
        case 'c':                   // Course File Name
            if (++i >= argc)
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            // check for too long of a file anme
            else
                *ppszCourseFileName = argv[i];
            break;
        case 'q':                   // Query File Name
            if (++i >= argc)
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            else
                *ppszQueryFileName = argv[i];
            break;
        case '?':
            exitUsage(USAGE_ONLY, "", "");
            break;
        default:
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
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
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    In general, this routine optionally prints error messages and diagnostics.
    It also prints usage information.

    If this is an argument error (iArg >= 0), it prints a formatted message 
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits 
    with ERR_COMMAND_LINE.

    If this is a usage error (but not specific to the argument), it prints 
    the specific message and its supplemental diagnostic information.  It 
    also shows the usage and exist with ERR_COMMAND_LINE. 

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript or control:
                                    > 0 - command argument subscript
                                    0 - USAGE_ONLY - show usage only
                                    -1 - USAGE_ERR - show message and usage
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    switch (iArg)
    {
        case USAGE_ERR:
            fprintf(stderr, "Error: %s %s\n"
                , pszMessage
                , pszDiagnosticInfo);
            break;
        case USAGE_ONLY:
            break;
        default:
            fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
                , iArg
                , pszMessage
                , pszDiagnosticInfo);
    }
    // print the usage information for any type of command line error
    fprintf(stderr, "p2 -c courseFileName -q queryFileName\n");
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY); 
    else 
        exit(ERR_COMMAND_LINE);
}
/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also
    returns the position in the text after that token.  This function
    does not skip over white space, but it assumes the input uses 
    spaces to separate tokens.
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
    - If the token is larger than iTokenSize, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
**************************************************************************/
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize)
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

    // if the delim position is at the first character, return no token.
    if (iDelimPos == 0)
        return NULL;

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
