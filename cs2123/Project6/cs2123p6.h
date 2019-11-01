/**********************************************************************
cs2123p5.h
Purpose:
   Defines constants: 
       max constants 
       error constants
       warning constants
       boolean constants
   Defines typedef for 
       EdgeNode  - graph edge containing preq and successor vertex
       Vertex    - contains course information (course number, name), existence boolean,
                   successor list first node pointer, predecessor list first node pointer
       GraphImp  - array of vertices and a count of them
       Graph     - pointer to an allocated GraphImp
       PlanImp   - tbd       
   Defines function prototypes for functions used in pgm5 (recursive and non-recursive)
   Defines function prototypes for functions used in pgm6 
   Defines WARNING macro
Notes:
   
**********************************************************************/
/*** constants ***/
#define MAX_TOKEN 50               // Maximum number of actual characters for a token
#define MAX_LINE_SIZE 100          // Maximum number of character per input line
#define MAX_VERTICES 60
#define OVERFLOW_BEGIN 29          // begin of overflow area 
// Error constants (program exit values)
#define ERR_COMMAND_LINE    900    // invalid command line argument
#define ERR_ALGORITHM      903     // Unexpected error in algorithm

#define ERR_TOO_MANY_COURSE 1      // Too many courses
#define ERR_BAD_COURSE 3           // Bad Course Data
#define ERR_BAD_PREREQ 4           // Bad Prereq Data
#define ERR_MISSING_SWITCH         "missing switch"
#define ERR_EXPECTED_SWITCH        "expected switch, found"
#define ERR_MISSING_ARGUMENT       "missing argument for"

// exitUsage control 
#define USAGE_ONLY          0      // user only requested usage information
#define USAGE_ERR           -1     // usage error, show message and usage information

// boolean constants
#define FALSE 0
#define TRUE 1

// EdgeNode represents one edge in a graph
typedef struct EdgeNode
{
    int iPrereqVertex;              // prereq
    int iSuccVertex;                // successor
    struct EdgeNode *pNextEdge;     // points to next  edge
} EdgeNode;


typedef struct Vertex
{
    char szCourseId[8];             // Course Identifier
    char szCourseName[21];          // Course Full Name
    char szDept[4];                 // Department (e.g., CS, MAT)
    int bExists;                    // pgm6 DELETE command causes this to be set to TRUE
                                    // TRUE - this vertex exists, FALSE - deleted
    EdgeNode * prereqList;
    EdgeNode * successorList;
    int iSemesterLevel;             // Which semester should this be taken
    int iHashChainNext;             // pgm 6 extra credit
    int iDistSource;                // Distance from a source
} Vertex;

// GraphImp of a double adjacency list graph
typedef struct
{
    int iNumVertices;             // Number of vertices in the vertexM array
    Vertex vertexM[MAX_VERTICES]; // Array of vertices
    int iOverflowBegin;           // The subscript of the first overflow entry
                                  // in the array of vertices.
                                  // Any subscript less than this value is in
                                  // the primary area.      
    int iFreeHead;                // Subscript of a free list of entries
                                  // in the overflow portion of the 
                                  // graph's vertexM array
} GraphImp;

typedef GraphImp *Graph;

// Degree Plan

typedef struct
{
    int semesterM[5][MAX_VERTICES]; // Array which has five rows for each semester.  
    int bIncludeM[MAX_VERTICES];    // TRUE if this course is to be included in the plan
} PlanImp;
typedef PlanImp * Plan;


// Prototypes
Vertex *newVertex( );
EdgeNode *allocEdgeNode( );
void linkCourseSucc(Graph graph, EdgeNode *e1, EdgeNode *e2, int iVertex);
void linkCoursePrereq(Graph graph, EdgeNode *e1, EdgeNode *e2, int iVertex);

// Recursive functions for program 5
int maxChain(Graph graph, int iVertex);
void printTraversal(Graph graph, int iCourseVertex, int indent);
void printLongChains(Graph graph, int iVertex, int pathM[], int iLevel, int iLongLength);
int causesCycle(Graph graph, int iPrereqVertex, int iVertex);

// Non-recursive for program 5
int findCourse(Graph graph, char szCourseId[]);
void insertPrereq(Graph graph, int iPrereqVertex, int iCourseVertex);
void printAllInList(Graph graph); 
void printOne(Graph graph, int iVertex);
void printSources(Graph graph);
void printSinks(Graph graph);

Graph newGraph();

int insertCourse(Graph graph, char szCourseId[]);
// Program 6 function for delete
void deleteCourse (Graph graph, int iVertex);

// Program 6 functions for Plan
void doPlan(Graph graph, Plan plan);
void setLevel(Graph g, Plan plan, int iVertex, int iLev);
Plan newPlan();

// hash function for extra credit
int hash(Graph g, char SzCourseId[]);
void printHash(Graph g);
void printChain(Graph g, int iVertex);

// functions in most programs, but require modifications
void processCommandSwitches(int argc, char *argv[], char **ppszCommandFileName);
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
