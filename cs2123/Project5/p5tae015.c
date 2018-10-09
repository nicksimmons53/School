/******************************************************************************
 p5tae015.c by Triston Scallan
 Purpose:
    This file contains the functions: printTraversal, causesCycle, printSources,
    and, printSinks. printTraversal pretty prints out all the successors of a
    given course and the successors of those. causesCycle determines if an
    insertion will create a loop from its successor(s) to its prereq,
    printSources prints all nodes that only produces edges and printSinks
    prints all all nodes that only terminates edges.
*****************************************************************************/

#include "cs2123p5.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/**************************** printTraversal **********************************
 void printTraversal(Graph graph, int iCourseVertex, int indent)

 Purpose:
    This is invoked due to the PRTSUCC command. printTraversal is a recursive
    function which prints the current vertex information (course ID, course
    Name) and then uses a depth first traversal to indent and print the
    successors.
 Parameters:
    Graph       graph               Graph struct
    int         indent              # of indentions for pretty print
    int         iCourseVertex       Subscript of the current course vertex
 Returns:
    N/A
*****************************************************************************/
void printTraversal(Graph graph, int iCourseVertex, int indent)
{
    Vertex v = graph->vertexM[iCourseVertex];   //Vertex shortcut
    EdgeNode *e;                                //edgenode shortcut
    int i;

    //create the indentions and then print the contents of this vertex
    for (i = 0; i < indent; i++)
    {
        printf("\t");
    }
    printf("%s%s %s\n", v.szDept, v.szCourseId, v.szCourseName);

    //iterate through the successor list, recursively going deeper each time.
    for (e = v.successorList; e != NULL; e = e->pNextEdge)
    {
        printTraversal(graph, e->iSuccVertex, indent + 1);
    }
}

/**************************** causesCycle *************************************
int causesCycle(Graph graph, int iPrereqVertex, int iVertex)

Purpose:
    Returns TRUE if the insertion of an edge containing the course
    and its prereq would cause a cycle. This is used by the PREREQ command to
    check whether the insertion of a prereq would cause a cycle prior to
    actually inserting that prereq.
Parameters:
    Graph       graph               Graph struct
    int         iVertex             Subscript of the current vertex
    int         iPrereqVertex       Subscript of the prereq vertex
Notes:
    there would be a cycle if a traversal to successors of iVertex
    reaches iPrereqVertex
Returns:
    int iResults - A true or false boolean based on if the graph could loop
*****************************************************************************/
int causesCycle(Graph graph, int iPrereqVertex, int iVertex)
{
    //traverse along the graph from point iVertex to the next successor
    int iResult = FALSE;                                //result variable
    EdgeNode *e = graph->vertexM[iVertex].successorList;//shortcut variable

    if (iVertex == iPrereqVertex)   //if this vertex matches the prereq; return.
        return TRUE;
    if (e == NULL)                  //if this vertex has no successors; return.
        return FALSE;

    //dive through all successors until it hits a sink, then back up and
    //try the next edge-successor until all successors are exhausted.
    for (; e != NULL; e = e->pNextEdge)
    {
        if (iResult)    //if a match was found, keep returning TRUE to escape.
            return iResult;
        iResult = causesCycle(graph, iPrereqVertex, e->iSuccVertex);
    }
    return iResult;
}

/**************************** printSources ************************************
 void printSources (Graph graph)

 Purpose:
    This function will print out courses that are "sources", i.e. courses with
    no prereqs and 1+ successor.
 Parameters:
    Graph       graph               Graph struct
 Returns:
    N/A
*****************************************************************************/
void printSources(Graph graph)
{
    int i;

    //iterate through all vertices of the graph and print if matches are found
    printf("%2s %-12s %-s\n", "Vx", "Course", "Name");
    for (i = 1; i <= graph->iNumVertices; i++)
    {
        Vertex v = graph->vertexM[i];
        //if the vertex has successor and doesn't have prereqs, then print it
        if (v.successorList != NULL && v.prereqList == NULL)
        {
            printf("%2d %s%-7s\t%-s\n",
                i, v.szDept, v.szCourseId, v.szCourseName);
        }
    }
}

/**************************** printSinks **************************************
 void printSinks (Graph graph)

 Purpose:
    This function will print out courses that are "sinks", i.e. courses with no
    successors but has prereqs that can point to it.
 Parameters:
    Graph       graph               Graph struct
 Returns:
    N/A
*****************************************************************************/
void printSinks(Graph graph)
{
    int i;

    //iterate through all vertices of the graph and print if matches are found
    printf("%2s %-12s %-s\n", "Vx", "Course", "Name");
    for (i = 1; i <= graph->iNumVertices; i++)
    {
        Vertex v = graph->vertexM[i];
        //if the vertex has prereqs and doesn't have successors, then print it
        if (v.successorList == NULL && v.prereqList != NULL)
        {
            printf("%2d %s%-7s\t%-s\n",
                i, v.szDept, v.szCourseId, v.szCourseName);
        }
    }
}
