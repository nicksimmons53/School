/******************************************************************************
p5qzn876.c by Charles Orlowski
Purpose:
	This file contains the functions: maxChain, printLongChains,printOne, and
	printAllInList. The function maxChain prints the max number of chains at the
	given vertex. The function printLongChains prints the chains that are the
	given length at the given vertex. The funtion printOne prints the specified
	vertex's information. The funtion printAllInList calls printOne for each
	vertex.
Command Parameters:
	int maxChain(Graph graph, int iVertex)
	void printLongChains(Graph graph, int iVertex, int pathM[]
		, int iLevel, int iLongLength)
	void printAllInList(Graph graph)
	void printOne(Graph graph, int iVertex)
Results:
	The maxChain produces the longes chain length. printLongChains prints the
	longest chains. printOne prints one line of a vertex's information.
	printAllInList prints information for every vertex.
Returns:
 	maxChain returns the max chain length
Notes:
	None.
 *****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

/**************************maxChain********************************************
	int maxChain(Graph graph, int iVertex)
Purpose:
	goes through an ajacency list depth first to find the max chain
	of the list at a specific vertex
Parameters:
	I       Graph   graph       structered ajacency list
	I/0     int     iVertex     subscript for the current vertex
Notes:

Return Value:
	returns value max
******************************************************************************/
int maxChain(Graph graph, int iVertex)
{
	// temperary node
	EdgeNode *e;
	// iCurr is the current depth of the chain
	// iTemp is a temp value for the maxchain
	// iCount is the subscript used in the array
	int iCur = 0, iTemp = 0, iMax = 0, iCount = 0;
	// iArr is Array Containing the current levels max values
	int iArr[70]; // MAX_VERTICES = 70

	//check if the next vertex is NULL return 1
	if(graph->vertexM[iVertex].successorList == NULL)
		return 1;
	//increment the current depth
	iCur += 1;
	// loop to recursivily go through maxChain and store those amounts in iArr
	for(e = graph->vertexM[iVertex].successorList; e != NULL; e = e->pNextEdge)
	{
		iTemp = maxChain(graph, e->iSuccVertex);// recursivily calls max chain
		iCur += iTemp;// add the max found in maxChain call to iCur
		iArr[iCount] = iCur;// store the iCur in the array
		iCur -= iTemp;// reset the iCur to before the maxChain Call
		iCount +=1;// increase the iArry subscript
	}
	// loop to compare the values in iArr to found the max
	for(int i = 0; i < iCount; i++)
	{
		// set the new max if requirement met
		if(iArr[i] > iMax)
			iMax = iArr[i];
	}
  // return the max values
	return iMax;
}
/**************************printLongChains*************************************
  void printLongChains(Graph graph, int iVertex, int pathM[]
		, int iLevel, int iLongLength)
Purpose:
  goes through the ajacency list depth first and prints the chains that
	are the longest lenght at the starting vertex recursivily
Parameters:
   I       Graph   graph        structered ajacency list
	 I/O     int     iVertex      subscript for the current vertex
   I/O     int     pathM[]      array showing the path from the
	                              starting vertex
   I/O     int     iLevel       the subscript for the pathM[] array
   I       int     iLongLength  known longest chain length
Notes:
	the 0 position in pathM array is set in the driver

******************************************************************************/
void printLongChains(Graph graph, int iVertex, int pathM[]
	, int iLevel, int iLongLength)
{
	// temperary node
	EdgeNode *e;
	// int i and j are integares for the for loops to increment through
	// iTemp is set to integers in the pathM array
    int i = 0,j = 0, iTemp = 0;

    // if the next vertex is NULL and if the current vertex distance from the
    // starting vertex is equeal to the longest chain found
	if(graph->vertexM[iVertex].successorList == NULL)
	{
        // if the iLevel plus one matches the longest chain print out the
        // integers in pathM array
		if(iLevel + 1 == iLongLength)
		{
			// go throught the pathM array print the department
            // and course id at the specified vertex
            printf("\t\t");
            for(int j = 0; j <= iLevel; j++)
            {
				// sets iTemp to a vertex subscript in pathM array
				iTemp = pathM[j];
                // prints the department and course id
                printf("%s%s "
                       , graph->vertexM[iTemp].szDept
                       , graph->vertexM[iTemp].szCourseId);
            }
			// next line print
            printf("\n");
            return;
		}
	}
    // increment the depth of the recursion
	iLevel += 1;
	// loop to go through the ajacency list recursivily
	for(e = graph->vertexM[iVertex].successorList; e != NULL; e = e->pNextEdge)
	{
		// set the current vertex subscript in the pathM array
        pathM[iLevel] = e->iSuccVertex;
		// recursive call for printLongChains
        printLongChains(graph, e->iSuccVertex, pathM, iLevel, iLongLength);
	}
	return;
}

/**************************printAllInList**************************************
    void printAllInList(Graph graph)
Purpose:
	calls printOne for every vertice in the graph to print its informantion
	and its prereqs and successor courses information
Parameters:
	I		Graph   graph 		structered ajacency list
Notes:

******************************************************************************/
void printAllInList(Graph graph)
{
	// int for the for loop to incriment
	int i;
	// prints out the header
	printf("%2s %2s %-7s %-20s%-31s %-s\n",
	    "Vx", "TE", "Course", "Name", "Prereqs", "Successors");
	//loop for the printOne call
	for(i = 1; i <= graph->iNumVertices; i++)
	{
		// calls print function
		printOne(graph, i);
	}
	return;
}

/**************************printOne********************************************
    void printOne(Graph graph, int iVertex)
Purpose:
    prints the courses information at the vertex passed and its prereq list and
		successor list vertexs information
Parameters:
  I       Graph   graph       structered ajacency list
	I/O     int     iVertex     subscript for the current vertex
Notes:

******************************************************************************/
void printOne(Graph graph, int iVertex)
{
	// nodes for the prereq and successor lists
	EdgeNode *p,*s;
	// iTemp is a temperary value for the prereq or successor list vertex
	// iCnt is a count for the number of prereq courses to adjust the tab space
	int iTemp, iCnt=0;
	// szCombName is an array to combine the department and course id for printing
    char szCombName[8];
	//adds department and course id to szCombName
    strcpy(szCombName, graph->vertexM[iVertex].szDept);
    strcat(szCombName, graph->vertexM[iVertex].szCourseId);
	// prints the iVertex's course name, department, and id
	printf("%2d %2d %-7s %-20s"
		,iVertex
		,graph->vertexM[iVertex].iDistSource
        ,szCombName
		,graph->vertexM[iVertex].szCourseName);
	// for loop that goes through the prereq list
	for(p = graph->vertexM[iVertex].prereqList; p != NULL; p = p->pNextEdge)
	{
		// sets itemp to the current prereq vertex
		iTemp = p->iPrereqVertex;
		//adds department and course id to szCombName
        strcpy(szCombName, graph->vertexM[iTemp].szDept);
        strcat(szCombName, graph->vertexM[iTemp].szCourseId);
        // prints a prereq vertex's info
        printf("%-7s ", szCombName);
        // increments iCnt
        iCnt++;
	}
	// sets the number of blank space symbols printed based on the iCnt count
	if(iCnt == 0)
	{
		printf("%-7s %-7s %-7s %-7s ", "-", "...", "...", "...");
	}
	else if (iCnt == 1)
	{
		printf("%-7s %-7s %-7s ", "...", "...", "...");
	}
	else if (iCnt == 2)
	{
		printf("%-7s %-7s ", "...", "...");
	}
	else if (iCnt == 3)
	{
		printf("%-7s ", "...");
	}

    //prints a dash if successor does not exist
    if(graph->vertexM[iVertex].successorList == NULL)
        printf("-");
	// for loop that goes through the successor list
	for(s = graph->vertexM[iVertex].successorList; s != NULL; s = s->pNextEdge)
	{
        // sets iTemp to the vertex of the successor list
        iTemp = s->iSuccVertex;
		//adds department and course id to szCombName
        strcpy(szCombName, graph->vertexM[iTemp].szDept);
        strcat(szCombName, graph->vertexM[iTemp].szCourseId);
        // prints a successor list vertex's info
        printf("%-7s ", szCombName);
	}
	// next line print
	printf("\n");
	return;
}
