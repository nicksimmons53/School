/******************************************************************************
p5Driver.c by Kevin Heston
Purpose:
    This program reads in input and creates a graph, it then manipulates and
    displays the data contained in that graph based on commands from the input.
Command Parameters:
    n/a
Input:
    The standard input stream contains commands (one per input text line) 
    and any relevant data.
Results:
    The program will display information contained within the graph based on 
    the command inputs.
Returns:
    0 - normal
Notes:
    This program stores the vertices of the graph inside of an array and keeps
    track of successors and prerequisites using a double adjacency list
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p6.h"

int main()
{
    char szInputBuffer[100] = "";       //contains the current input
    char szCommandToken[MAX_TOKEN+1];   //conatins the current command
    char *pszRemainingInput;            //indicates where remaining input starts
    int iScanfCnt = 0;                  //tracks successful sscanf reads
    int iCourseIndex = 0;               //current course index
    
    char szCourseId[8];                 //current course ID number
    char szCourseName[21];              //current course name
    char szCourseDept[4];               //current course department

    char szPrereqId[8];                 //current prerequisite ID number
    char szPrereqDept[4];               //current prerequisite department
    int iPrereqIndex = 0;               //current prerequisite index

    Vertex * pVertex;
    
    //allocates a new graph
    Graph g = newGraph();
    
    //reads in lines of input and performs the relevant command
    while(fgets(szInputBuffer, 100, stdin) != NULL)
    {
        //skips empty lines
        if(szInputBuffer[0] == '\n')
            continue;
        
        //stores the current command in szCommandToken and points
        //pszRemainingInput to the start of the rest of the input
        pszRemainingInput = getToken(szInputBuffer, szCommandToken, MAX_TOKEN);
        
        //prints the current line of input
        printf(">> %s", szInputBuffer);
        
        //determines what action should be taken for the current command
        if(strcmp(szCommandToken,"COURSE") == 0)
        {
            //scans in the course info and checks that the scan was successful
            iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9] %[^\n]\n",
                            szCourseDept, szCourseId, szCourseName);
            if(iScanfCnt != 3)
            {
                printf("Expected 3 values from COURSE command input "
					   "and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
                printf("Dept:%s ID:%s Name: %s\n", szCourseDept, szCourseId, szCourseName);
                printf("%s\n", pszRemainingInput);
            }
            
            //searches the vertex array to see if course already exists
            iCourseIndex = findCourse(g, szCourseId);
            
            //adds course if it does not exist and changes name otherwise
            if(iCourseIndex == -1)
            {						
		g->iNumVertices++;
                pVertex = newVertex(szCourseDept, szCourseId);
                g->vertexM[g->iNumVertices] = *pVertex;
                strcpy(g->vertexM[g->iNumVertices].szCourseName, szCourseName);
                free(pVertex);
               // g->vertexM[g->iNumVertices] = *newVertex(szCourseDept, szCourseId);
            }
            else
            {
                strcpy(g->vertexM[iCourseIndex].szCourseName, szCourseName);
            }
        }
        else if(strcmp(szCommandToken,"PREREQ") == 0)
        {
            //scans in remaining input and ensure that it's valid
            iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n",
                            szPrereqDept, szPrereqId);
            if(iScanfCnt != 2)
            {
                printf("Warning: Expected 2 values from PREREQ command "
			"input and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
            }
            
            //retrieve course and prerequisite indices
            iPrereqIndex = findCourse(g, szPrereqId);
            iCourseIndex = findCourse(g, szCourseId);

            //checks to see if insertion would cause a cycle
            //and if so prints a warning
            if(!causesCycle(g, iPrereqIndex, iCourseIndex))
            {
                //creates new vertex to represent prereq if necessary
                if(iPrereqIndex == -1)
                {
                    g->iNumVertices++;

                    pVertex = newVertex(szPrereqDept, szPrereqId);
                    g->vertexM[g->iNumVertices] = *pVertex;
                    //g->vertexM[g->iNumVertices] = *newVertex(szCourseDept, szCourseId);
                    insertPrereq(g, g->iNumVertices, iCourseIndex);
                    free(pVertex);
                }
                else
                {
                    insertPrereq(g, iPrereqIndex, iCourseIndex);
                }
            }
            else
            {
                printf("Warning: Insertion of this course would result " 
                       "in a cycle. Input: %s\n", szInputBuffer);
            }       
                
        }
        else if(strcmp(szCommandToken,"PRTONE") == 0)
        {
            //scans in remaining input and ensure that it's valid
	    iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n",szCourseDept, szCourseId);
            if(iScanfCnt != 2)
            {
                printf("Warning: Expected 2 values from PRTONE command "
					   "input and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
            }

            //retrieves index of current course
	    iCourseIndex = findCourse(g, szCourseId);
            
            //displays header for PRTONE command
            printf("%2s %2s %-7s %-20s%-31s %-s\n", 
                "Vx", "TE", "Course", "Name", "Prereqs", "Successors");
	    printOne(g, iCourseIndex);
	    printf("\n");
        }
        else if(strcmp(szCommandToken,"PRTALL") == 0)
        {
            printAllInList(g); 
	    printf("\n");
        }
        else if(strcmp(szCommandToken,"PRTSUCC") == 0)
        {
            //scans in remaining input and ensure that it's valid
	    iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n",szCourseDept, szCourseId);
            if(iScanfCnt != 2)
            {
                printf("Warning: Expected 2 values from PRTSUCC command "
		       "input and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
            }
            
            //retrieves current course index
	    iCourseIndex = findCourse(g, szCourseId);

            printTraversal(g, iCourseIndex, 0);
        }
        else if(strcmp(szCommandToken,"PRTSOURCES") == 0)
        {
            printSources(g);
        }
        else if(strcmp(szCommandToken,"DELETE") == 0)
        {
            iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n",szCourseDept, szCourseId);

            iCourseIndex = findCourse(g, szCourseId);

            deleteCourse(g, iCourseIndex);

            printf("%d", g->vertexM[iCourseIndex].bExists);
        }
        else if(strcmp(szCommandToken,"PRTSINKS") == 0)
        {
            printSinks(g);
        }
        else if(strcmp(szCommandToken,"MAXCHAIN") == 0)
        {					
            //scans in remaining input and ensure that it's valid
            iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n"
                               ,szCourseDept, szCourseId); 
            if(iScanfCnt != 2)
            {
                printf("Warning: Expected 2 values from MAXCHAIN command "
		       "input and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
            }
            
            //retrieves current course index
            iCourseIndex = findCourse(g, szCourseId);
            
            //displays the header and information for the maxChain command
            printf("\tMaximum chain for %s%s contains %i courses\n", 
                    szCourseDept, szCourseId, maxChain(g, iCourseIndex));
        }
        else if(strcmp(szCommandToken,"PRTLONGS") == 0)
        {
            //scans in remaining input and ensure that it's valid
            iScanfCnt = sscanf(pszRemainingInput, "%[A-Z]%[0-9]\n",
                               szCourseDept, szCourseId);
            if(iScanfCnt != 2)
            {
                printf("Warning: Expected 2 values from PRTLONGS command "
		       "input and received %d successful values. Input: %s\n",
                        iScanfCnt, szInputBuffer);
            }
            
            //creates a array to store path through graph
            int pathM[MAX_VERTICES];

            //retreives the current course index
            iCourseIndex = findCourse(g, szCourseId);

            //uses maxChain to determine the longest length
            int iLongLength = maxChain(g, iCourseIndex);
            int iLevel = 0;
            
            //sets the current course to be the first on the path
            pathM[0] = iCourseIndex;

            //displays the header for display of long chains
            printf("\tLongest Chains beginning with %s%s\n", 
                    szCourseDept,szCourseId);

            printLongChains(g, iCourseIndex, pathM, iLevel,
                            iLongLength);
            printf("\n");
        }
        else if(strcmp(szCommandToken,"*") == 0)
        {
            //does nothing since the "*" command acts as a comment
            continue;
        }
        else
        {
            printf("Warning: Unrecognized Command \"%s\"\n", szCommandToken);
        }
    }
    
    EdgeNode * e;
    EdgeNode * temp;
    for(int i = 1; i <= g->iNumVertices; i++)
    {
        for(e = g->vertexM[i].prereqList; e != NULL;)
        {
            temp = e->pNextEdge;
            free(e);
            e = temp;
        }

        for(e = g->vertexM[i].successorList; e != NULL;)
        {
            temp = e->pNextEdge;
            free(e);
            e = temp;
        }
        
    }
    free(g);

}

/******************** newVertex ***********************************************
Vertex * newVertex(char szDept[], char szCourseId[])
Purpose:
    Allocates a new vertex and returns a pointer to it
Notes:
    Checks that the malloc returned a valid address and initilizes course ID
    and course department. Also sets the course name to be "TBD"
Return Value:
    Vertex *    pointer to the newly allocated vertex
******************************************************************************/
Vertex * newVertex(char szDept[], char szCourseId[])
{
    //allocates memory and ensures it was allocated correctly
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    if(v == NULL)
            ErrExit(ERR_ALGORITHM, "No memory available for new vertex");

    //initializes course info
    strcpy(v->szCourseId, szCourseId);
    strcpy(v->szDept, szDept);
    strcpy(v->szCourseName, "TBD");

    v->iSemesterLevel = 0;
    v->iHashChainNext = 0;
    v->iDistSource = 0;
    v->bExists = TRUE;

    v->prereqList = NULL;
    v->successorList = NULL;

    return v;
}

/****************************** newGraph ***************************************
Graph newGraph()
Purpose:
    Allocates a new graph and returns a pointer to it
Notes:
    Checks that the malloc returned a valid address and initializes the
    iNumVertices to 0.
Return Value:
    Graph   pointer to the newly allocate graph
*******************************************************************************/
Graph newGraph()
{
    //allocates memory and ensures it was allocated correctly
    Graph g = (Graph)malloc(sizeof(GraphImp));
    if(g == NULL)
            ErrExit(ERR_ALGORITHM, "No memory available for new graph");

    g->iNumVertices = 0;

    return g;
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
