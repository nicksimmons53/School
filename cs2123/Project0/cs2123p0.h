/**********************************************************************
cs2123p0.h
Purpose:
   Defines the typedefs:
       Attr
       Course
   Defines maximum size constants
   Defines error constants
   Defines function prototypes used only for this program.
   Defines WARNING macro.
Notes:
Description of any special information around usage.
**********************************************************************/

/* constants */
#define MAX_COURSE 30          // Maximum number of courses
#define MAX_ATTR 12            // Maximum number of attributes per course       
#define MAX_LINE_SIZE 100      // Maximum number of character per input line

#define ERR_TOO_MANY_COURSE 1  // Too many courses
#define ERR_TOO_MANY_ATTR 2    // Too many attributes
#define ERR_BAD_INPUT 3        // Bad input 
#define ERR_ALGORITHM 999      // Unexpected error in algorithm

#define TRUE 1
#define FALSE 0

/* typedefs and function prototypes */

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
    char szAttrValue[13];       // This corresponds to the Attr Type:
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

int getCourseData(Course courseM[]);
void printCourseData(Course courseM[], int iNumCourse);

char * getToken(char *pszInputTxt, char szToken[], int iTokenSize);
void ErrExit(int iexitRC, char szFmt[], ...);

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

