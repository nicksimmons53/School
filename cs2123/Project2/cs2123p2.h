/**********************************************************************
cs2123p2.h
Purpose:
   Defines constants: 
       max constants 
       error constants
       warning constants
       categories of tokens (operator, operand, etc.)
       boolean constants
   Defines typedef for 
       Token
       Element          (values placed in stack or out)
       StackImp         (array stack implementation)
       Stack            (pointer to a StackImp)
       PostfixOutImp    (out implementation)
       PostfixOut       (pointer to an PostfixOutImp)
       Attr             (for defining attributeType and corresponding value))
       Course           (course information except Attr)
    Defines function prototypes used only for this program.
    Defines WARNING macro
Notes:
   
**********************************************************************/
/*** constants ***/
// Maximum constants
#define MAX_STACK_ELEM 20          // Maximum number of elements in the stack array
#define MAX_TOKEN 50               // Maximum number of actual characters for a token
#define MAX_OUT_ITEM 50            // Maximum number of PostfixOut items
#define MAX_COURSE 30              // Maximum number of courses
#define MAX_ATTR 12                // Maximum number of attributes per course       
#define MAX_LINE_SIZE 100          // Maximum number of character per input line

// Error constants (program exit values)
#define ERR_COMMAND_LINE    900    // invalid command line argument
#define ERR_STACK_USAGE    901
#define ERR_OUT_OVERFLOW   902
#define ERR_ALGORITHM      903     // Unexpected error in algorithm

#define ERR_TOO_MANY_COURSE 1      // Too many courses
#define ERR_TOO_MANY_ATTR 2        // Too many attributes
#define ERR_BAD_COURSE 3           // Bad Course Data

#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found"
#define ERR_MISSING_ARGUMENT        "missing argument for"

// exitUsage control 
#define USAGE_ONLY          0      // user only requested usage information
#define USAGE_ERR           -1     // usage error, show message and usage information

// Warning constants.  Warnings do not cause the program to exit.
#define WARN_MISSING_RPAREN 801
#define WARN_MISSING_LPAREN 802
#define WARN_MISSING_OPERATOR 803
#define WARN_MISSING_OPERAND 804

// categories of tokens
#define CAT_LPAREN 1        // (
#define CAT_RPAREN 2        // )
#define CAT_OPERATOR 3      // Operators are =, NEVER, ONLY /
#define CAT_OPERAND 4       // These are attribute types or values

// boolean constants
#define FALSE 0
#define TRUE 1

/*** typedef ***/
/* Attr typedef defines a attribute type (e.g., DEPT) and
** corresponding attribute value (e.g., CS, MAT)
*/
typedef struct
{
    char szAttrType[11];        // Attr Type can be one of:
    //    DEPT
    //    PREREQ
    //    PROF
    //    LANG
    //    RECIT 
    char szAttrValue[MAX_TOKEN+1];       // This corresponds to the Attr Type:
    //    DEPT:     CS, MAT
    //    RECIT:    Y, N
    //    LANG:     C, JAVA, LISP, CPP, PYTHON, BASH, IA32
    //    PROF:     CLARK, MAYNARD, GIBSON, KORKMAZ, ROBBINS, ROBINSON, ...
    //              
    //    PREREQ:   CS1083, CS1713, CS2123, MAT1223, MAT1214, MAT3333,
    //              MAT2233, CS3443, CS3723, CS3843
} Attr;

/* Course typedef contains course Id, course name, and an array of attributes */
typedef struct
{
    char szCourseId[8];               // Course Identifier
    char szCourseName[21];            // Course Full Name
    int  iNumberOfAttrs;              // Particular number of attributes for each course
    Attr attrM[MAX_ATTR];
} Course;

typedef int QueryResult;

int getCourseData(Course courseM[]);
void printCourseData(Course courseM[], int iNumCourse);
// Token typedef used for operators, operands, and parentheses
typedef char Token[MAX_TOKEN + 1];

// Element typedef used for Element values placed in the stack or out
// The evaluation stack can have two types of data.
typedef union
{
    struct
    {
        Token szToken;      // token from the query
        int iCategory;      // category CAT_LPAREN, CAT_RPAREN, CAT_OPERATOR, CAT_OPERAND
        int iPrecedence;    // operator precedence with 0 being lowest
    };
    struct
    {
        Token szBoolean;    // This is to aid in debugging
        int bInclude;       // TRUE - intermediate include result, FALSE - intermediate don't include
    };
} Element;

// StackImp typedef defines how we implement a stack using an array
typedef struct
{
    int iCount;             // number of elements in stack.  0 is empty 
    Element stackElementM[MAX_STACK_ELEM];
} StackImp;

// Stack typedef defines a pointer to a stack
typedef StackImp *Stack;

// PostfixOutImp typedef defines how we implement out
typedef struct
{
    int iPostfixOutCount;
    Element postfixOutM[MAX_OUT_ITEM];
} PostfixOutImp;

// PostfixOut typedef defines a pointer to out
typedef PostfixOutImp *PostfixOut;

/**********   prototypes ***********/

// functions that each student must implement
// (Note that the student might implement several others for modularity.)

// your code from program #0
void printCourseData(Course courseM[], int iNumCourse);

// your code from program #1 (and other functions for modularity)
int convertToPostfix(char *pszInfix, PostfixOut postfixOut);

// your code for program #2
void evaluatePostfix(PostfixOut postfixOut, Course courseM[], int iNumCourse, QueryResult resultM[]);
int atLeastOne(Course *pCourse, Attr attr);
int only(Course *pCourse, Attr attr);

// Stack functions that Larry provided

void push(Stack stack, Element value);
Element pop(Stack stack);
int isEmpty(Stack stack);
Stack newStack();
void freeStack(Stack stack);
Element topElement(Stack stack);

// Conversion to Postfix functions that Larry provided
void categorize(Element *pElement);
void addPostfixOut(PostfixOut postfixOut, Element element);
void printPostfixOut(PostfixOut postfixOut);

// Evaluation functions that Larry provided
void printQueryResult(Course courseM[], int iNumCourse, QueryResult resultM[]);
int getCourseData(Course courseM[]);
void readAndProcessQueries(Course courseM[], int iNumberOfCourses);
int never(Course *pCourse, Attr attr);

// functions in most programs, but require modifications
void processCommandSwitches(int argc, char *argv[], char **ppszCourseFileName
    , char **ppszQueryFileName);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);

// Utility routines provided by Larry
void ErrExit(int iexitRC, char szFmt[], ...);
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize);

/* 
  WARNING macro 
  Parameters:
     I szFmt - a printf format
     I ...   - a variable number of parameters corresponding to szFmt's
               format codes.
  Results:
     Prints "WARNING" and the value(s) specified by the szFmt.
  Notes:
     Since this generates multiple C statements, we surround them
     with a dummy do while(0) which only executes once.  Notice that the
     dummy do while isn't ended with a ";" since the user of 
     the macro naturally specifies a ";".  Example:
         if (x != 0)
             WARNING("X must be blah blah");
         else
         {   // normal processing
            ....
         } 
     If we used {} in the macro definition instead of the dummy do while(0),
     the generated code would have a bad ";":
         if (x != 0)
         {
             printf("\tWARNING: "); 
             printf("X must be blah blah");
             printf("\n");
         } ;  // yuck, bad ";" causing the compiler to not understand the else
         else
         {   // normal processing
            ....
         } 
*/
#define WARNING(szFmt, ...) do {    \
    printf("\tWARNING: ");          \
    printf(szFmt, __VA_ARGS__);     \
    printf("\n");                   \
    } while (0)
