/******************************************************************************
 p5srz888.c by Nicholas Simmons
 Purpose:
    This file contains the functions: insertPrereq, findCourse, and 
    allocEdgeNode. The function insertPrereq will construct the edges used to
    create connections between Course vertices and their Prerequisites. 
    findCourse will search the vertex array for a matching vertex and return
    the subscript. allocEdgeNode allocates the memory required for the EdgeNodes
    used in insertPrereq.
 Results:
    To construct edges that will connect vertices in a graph of courses.
 Notes:
    None.
 *****************************************************************************/

#include "cs2123p5.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/**************************** insertPrereq ************************************
 void insertPrereq(Graph graph, int iPrereqVertex, int iCourseVertex)
 
 Purpose:       This function will insert an edge into the graph and connect
                the correct vertices using an edge. It will change not just the 
                Prereq List, but also the Successor List.
 Parameters:    Graph       graph               Graph struct that is being 
                                                inserted into
                int         iPrereqVertex       Subscript of the prereq vertex
                int         iCourseVertex       Subscript of the current course
                                                vertex
 Notes:         None.
 Returns:       None.
 *****************************************************************************/

void insertPrereq(Graph graph, int iPrereqVertex, int iCourseVertex)
{
	EdgeNode *p;
	EdgeNode *s;
	
	// Allocated memory for the current course and prereq edges
	p = allocEdgeNode( );
	s = allocEdgeNode( );
	
	// Set the course and prereq next edges to the correct vertex list
	p->pNextEdge = graph->vertexM[iCourseVertex].prereqList;
	s->pNextEdge = graph->vertexM[iPrereqVertex].successorList;
	
	graph->vertexM[iCourseVertex].prereqList = p;
	graph->vertexM[iPrereqVertex].successorList = s;
	
	// Set the current courses prereq and succ to correct values
	p->iPrereqVertex = iPrereqVertex;
	p->iSuccVertex = iCourseVertex;
	
	// Set the prereq courses prereq and succ to correct values
	s->iPrereqVertex = iPrereqVertex;
	s->iSuccVertex = iCourseVertex;
}


/**************************** findCourse **************************************
 int findCourse(Graph graph, char szCourseId[ ])
 
 Purpose:       This function will search the Vertex array and search for a 
                matching string comparing the Course IDs.
 Parameters:    Graph       graph               Graph struct containing the 
                                                vertex array that is being 
                                                searched.
                char        szCourseId[ ]       Course ID that is being searched
                                                for
 Notes:         None.
 Returns:       int         i                   The subscript of the matching 
                                                course if it is found.
                int         -1                  If the course is not found.
 *****************************************************************************/

int findCourse(Graph graph, char szCourseId[ ])
{
    // Variable for iteration
    int i;
    
    // Loop that iterates through the vertex array
    for (i = 1; i <= graph->iNumVertices; i++)
    {
        // If the course id matches return i
        if (strcmp(graph->vertexM[i].szCourseId, szCourseId) == 0)
            return i;
    }
    
    // If not found return -1
    return -1;
}

/**************************** allocEdgeNode ***********************************
 EdgeNode *allocEdgeNode( )
 
 Purpose:       Allocates memory for an Edge Node that is going to be inserted
                into the graph.
 Parameters:    None.
 Notes:         None.
 Returns:       EdgeNode    *pNew               A pointer to the newly allocated
                                                node.
 *****************************************************************************/

EdgeNode *allocEdgeNode( )
{
    EdgeNode *pNew;
    pNew = (EdgeNode *)malloc(sizeof(EdgeNode));
    
    if (pNew == NULL)
        ErrExit(ERR_ALGORITHM, "No Available Memory for a New EdgeNode");
    
    pNew->pNextEdge = NULL;
    
    return pNew; 
}
