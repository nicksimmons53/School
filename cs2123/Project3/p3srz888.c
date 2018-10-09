/**************************************************************************************************
p4srz888.c by Nicholas Simmons
Purpose:
    This program will create an ordered singly linked list for managing timed events. It will
    simulate arrivals and departures at different "clock" times. It will advance the clock based
    on the next event time. The simulation will iterate through the linked list and print the 
    corresponding output according to the type of event. 
Command Parameters:
    None.
Input:
    The program will read in output from the pFilePeople, which contains a persons name, departure
    time and the next persons arrival time. 
Results:
    It will print the persons arrival and departure time in chronological order.
Returns:
    None.
Notes:
    None.
**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cs2123p4.h"

/************************************************ pFilePeople *************************************
FILE *pFilePeople;
Purpose:
    Contains information that is going to be read into the program. The information is a name, 
    an integer that is the departure time of a widget in the simulation, and an arrival delta for 
    the next available widget.
Notes:
    None.
**************************************************************************************************/
FILE *pFilePeople;

// Prototypes for this file
NodeLL *getPeopleData(LinkedList list);
void freeNode(LinkedList list, NodeLL *pNode);

// MAIN FUNCTION FOR THE PROGRAM
//
int main( )
{
    // Allocate the memory for the sim structure and eventList
    Simulation sim = malloc(sizeof sim);
    sim->eventList = newLinkedList( );
    
    // Sort the linked list by iEventTime
    // sort(sim->eventList);
    
    getPeopleData(sim->eventList);

    // Run the simulation
    runSimulation(sim);
    
    free(sim);
    free(sim->eventList);
    return 0;
}

/************************************** runSimulation *********************************************
void runSimulation(Simulation simulation)

Purpose:        This function will loop through the linked list events, which are inserted and 
                sorted according to the iEventTime. It will then print the time, widget, and 
                event type. It prints those variables according to the node's iEventType. The 
                function then sets the sim->iClock to the current time of the event. 
Parameters:     Simulation sim      The sim struct contains the linked list that is being iterated
                                    through and the iClock that is being advanced.
Notes:          The linked list is iterated through using removeLL. As long as removeLL is 
                returning true, the function will continue until termination. 
Returns:        None.
**************************************************************************************************/
void runSimulation(Simulation sim)
{
    // Simple function variables
    Event event;
    Event event2;
    char arrive[15] = "Arrive";
    char depart[15] = "Depart";
    char simulation[15] = "Simulation";
    char terminates[15] = "Terminates";
    
    // Remove the node at the front of the linked list and check its iEventType
    while (removeLL(sim->eventList, &event))
    {
        // Advance the clock
        sim->iClock = event.iTime;

        // Check the event type and print the according message
        switch(event.iEventType)
        {
            case EVT_ARRIVE:
            {
                strcpy(event2.person.szName, event.person.szName);
                event2.iEventType = EVT_DEPART;
                event2.iTime = sim->iClock + event.person.iDepartUnits;
                insertOrderedLL(sim->eventList, event2);

                printf("%3d %-15s %-15s\n"
                    , sim->iClock
                    , event.person.szName
                    , arrive);
                break;
            }

            case EVT_DEPART:
            {
                printf("%3d %-15s %-15s\n"
                    , sim->iClock
                    , event.person.szName
                    , depart);
                break;
            }

            default:
            {
                ErrExit(ERR_ALGORITHM, "Unknown Event Type: %d\n", event.iEventType);
                break;
            }
        }
    }

    printf("%3d %-15s %-15s\n", sim->iClock, simulation, terminates);
}

/************************************** newLinkedList *********************************************
LinkedList newLinkedList( ) 

Purpose:        Allocate memory for a linked list and set the data to NULL.
Parameters:     None.
Notes:          None.
Returns:        LinkedList  list    linked list address
**************************************************************************************************/
LinkedList newLinkedList( )
{
    // Allocate the memory
    LinkedList list = (LinkedList)malloc(sizeof(LinkedListImp));

    // Set the list as empty
    list->pHead = NULL;

    return list;
}

/************************************** allocateNodeLL ********************************************
NodeLL *allocateNodeLL(LinkedList list, Event value)

Purpose:        Allocate memory for a new node that will be inserted into the Linked List.
Parameters:     LinkedList      list    Address to the linked list
                Event           value   Event struct that will be put into the new node
Notes:          None.
Returns:        NodeLL      pNew    Pointer to the new node that was allocated
**************************************************************************************************/
NodeLL *allocateNodeLL(LinkedList list, Event value)
{
    // New node that is allocated memory
    NodeLL *pNew;
    pNew = (NodeLL *)malloc(sizeof(NodeLL));
    
    // If the memory is not available ERROR
    if (pNew == NULL)
        ErrExit(ERR_ALGORITHM, "No available memory for linked list");
    
    // Set the node's Event variable to the value 
    // Set the next node in the list to NULL
    pNew->event = value;
    pNew->pNext = NULL;
    
    // Return the allocated node
    return pNew;
}

/************************************** insertOrderedLL *******************************************
NodeLL *insertOrderedLL(LinkedList list, Event value)

Purpose:        Insert a node into the correct position in an already sorted linked list.
Parameters:     LinkedList      list    Address to the linked list
                Event           value   Event struct that will be inserted as a node into the list
Notes:          None
Returns:        NodeLL      pNew        Node that is inserted into the linked list
**************************************************************************************************/
NodeLL *insertOrderedLL(LinkedList list, Event value)
{
    // Node declarations
    NodeLL *pNew;
    NodeLL *pFind;
    NodeLL *pPrecedes;

    pFind = searchLL(list, value.iTime, &pPrecedes);

    // Allocate the new node
    pNew = allocateNodeLL(list, value);
    
    // If the list is empty insert at the beginning 
    // Else insert it after the last node
    if (pPrecedes == NULL)
    {
        pNew->pNext = list->pHead;
        list->pHead = pNew;
    }
    else 
    {
        pNew->pNext = pPrecedes->pNext;
        pPrecedes->pNext = pNew;
    }

    // Return the inserted node
    return pNew;
}

/************************************** searchLL ************************************************** 
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)

Purpose:        Searches the linked list for a node that matches what is wanting to be found.
Parameters:     LinkedList      list            Address to the linked list.
                int             match           Key that is being searched for.
                NodeLL          **ppPrecedes    Pointer to the pointer of the address of the 
                                                preceding node.
Notes:          None.
Returns:        NodeLL      p       The node containing the matching key.
                NULL                If the key is not found in the list.
**************************************************************************************************/
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
{
    // Node variable for the for loop
    NodeLL *p;

    *ppPrecedes = NULL;
    
    // Loop to iterate through the nodes in the linked list
    for (p = list->pHead; p != NULL; p = p->pNext)
    {
        // If the match is found, return the node it's in
        if (match == p->event.iTime)
            return p;
        
        // Return the previous node before it
        if (match <= p->event.iTime)
            return NULL;

        *ppPrecedes = p;
    }
    
    // Return NULL if not found
    return NULL;
}

/************************************** removeLL **************************************************
int removeLL(LinkedList list, Event *pValue)

Purpose:        Remove a node from the front of the linked list.
Parameters:     LinkedList      list        Address to the linked list.
                Event           *pValue     Value that holds onto the data that is to be removed.
Notes:          None.
Returns:        FALSE   0
                TRUE    1
**************************************************************************************************/
int removeLL(LinkedList list, Event *pValue)
{
    // The node that will be removed
    NodeLL *pRemove;

    if (list->pHead == NULL)
        return FALSE;
    
    // Pull the data from the node and free just the node
    *pValue = list->pHead->event;
    pRemove = list->pHead;
    list->pHead = list->pHead->pNext;

    freeNode(list, pRemove);

    return TRUE;
}

/************************************** freeNode **************************************************
void freeNode(LinkedList list, NodeLL *pNode)

Purpose:        Frees the memory associated with a specific node.
                NodeLL          *pNode      The node whose data is to be freed.
Notes:          None.
Returns:        None.
**************************************************************************************************/
void freeNode(LinkedList list, NodeLL *pNode)
{
    free(pNode);
}

/************************************** exitUsage *************************************************
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)

Purpose:        In general, this routine optionally prints error messages and diagnostics. It also
                prints usage information.

                If this is an argument error (iArg >= 0), it prints a formatted message showing 
                which argument was in error, the specified message, and supplemental diagnostic 
                information. It also shows the usage and exist with ERR COMMAND LINE.

                If this is just asking for usage (iArg will be NULL), the usage is shown. It exits 
                with USAGE ONLY.
Parameters:     int         iArg                    Command argument subscript or control:
                                                    >0 - command argument subscript
                                                    0 - USAGE_ONLY - show usage only
                                                    -1 - USAGE_ERR - show message and usage 
                char        *pszMessage             Error message to print
                char        *pszDiagnosticInfo      Supplemental diagnostic information.
Notes:          This routine cause the program to exit. 
Returns:        None.
**************************************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    switch (iArg)
    {
        case USAGE_ERR:
        {
            fprintf(stderr, "Error: %s %s\n"
                , pszMessage
                , pszDiagnosticInfo);
            break;
        }

        case USAGE_ONLY:
            break;

        default:
        {
            fprintf(stderr, "Error: Bad Argument #%d.  %s %s\n"
                , iArg
                , pszMessage
                , pszDiagnosticInfo);
        }
    }
    
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY);
    else 
        exit(ERR_COMMAND_LINE);
}

/************************************** ErrExit ***************************************************
void ErrExit(int ixitRC, char szFmt[ ], ...)

Purpose:        Prints an error message defined by the printf-like szFmt and the corresponding 
                arguments to that function. THe number of arguments after szFmt varies dependent on
                the format codes in szFmt. 
                It also exits the program with the specified exit return code.
Parameters:     int         iexitRC         Exit return code for the program.
                char        szFmt[ ]        This contains the message to be printed and format 
                                            codes (just like printf) for values that we want to 
                                            print. 
                ...                         A variable-number of additional arguments which 
                                            correspond to what is needed by the format codes in 
                                            szFmt.
Notes:          Prints "ERROR: " followed by the formatted error message specified in szFmt.
                Prints the file path and file name of the program having the error. This is the 
                file that contains this routine.
                Requires including <stdarg.h>
Returns:        Returns a program exit return code:     the value of iexitRC.
**************************************************************************************************/
void ErrExit(int iexitRC, char szFmt[ ], ...)
{
    va_list args;           // This is the standard C variable argument list type
    va_start(args, szFmt);  // This tells the compiler where the variable arguments begins. They 
                            // begin after szFmt
    printf("ERROR: ");
    vprintf(szFmt, args);   // vprintf receives a printf format string and a va_list argument
    va_end(args);           // Let the C environment know we are finished with the va_list
                            // argument
    exit(iexitRC);
}

/************************************** getToken **************************************************
char *getToken (char *pszInputTxt, char szToken[ ], int iTokenSize)

Purpose:        Examines the input text to return the next token. It also returns the position in 
                the text after that token. This function does not skip over white space, but it 
                assumes the input uses spcaes to separate tokens.
Parameters:     char    *pszInputTxt    input buffer to be parsed
                char    szToken[ ]      Returned token.
                int     iTokenSize      The size of the token variable. This is used to prevent 
                                        overwriting memory. The size should be the memory size 
                                        minus 1 (for the zero byte).
Notes:          If the token is larger than iTokenSize, we return a truncated value
                If a token isn't found, szToken is set to an empty string
                This function does not skip over white space occuring to the prior token.
Returns:        Functionally:
                    Pointer to the next character following the delimiter after the token.
                    NULL - no token found
                szToken parm - the returned token. If not found, it will be an empty string.
**************************************************************************************************/
char *getToken(char *pszInputTxt, char szToken[ ], int iTokenSize)
{
    int iDelimPos;                          // Found position of delim
    int iCopy;                              // Number of characters to copy
    char szDelims[20] = " \n\r";            // Delimiters
    szToken[0] = '\0';

    // Check for NULL pointer
    if (pszInputTxt == NULL)
        ErrExit(ERR_ALGORITHM
        , "getToken passed a NULL pointer");

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // Get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // If the delim position is at the first character, return no token
    if (iDelimPos == 0)
        return NULL;

    // See if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;
    else 
        iCopy = iDelimPos;

    // Copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';

    // Advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else 
        return pszInputTxt + 1;

}

/************************************** getPeopleData *********************************************
NodeLL *getPeopleData(LinkedList list)

Purpose:        This function opens the pFilePeople file, and reads its contents into an Event
                structure. That event structure is then insorted into the linked list and is 
                populated with the scanned data.
Parameters:     LinkedList      list        The linked list that is populated with scanned data.
Notes:          This function will print an error is the file is read incorrectly. 
Returns:        NodeLL      *p
**************************************************************************************************/
NodeLL *getPeopleData(LinkedList list)
{
    char szInputBuffer[MAX_LINE_SIZE + 1];
    FILE *pFilePeople;
    int iScanfCnt;
    Widget widget;
    Event event;
    char szToken[MAX_TOKEN];
    int iTime = 0;
    
    // Open the file for a read only
    pFilePeople = fopen("p4Input.txt", "r");

    if (pFilePeople == NULL)
        printf("Error: p3Input.txt could not be opened\n");

    // Retrieve the data from the file and insert it into the event structure
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pFilePeople) != NULL)
    {
        if (szInputBuffer[0] == '\n')
            continue;

        getToken(szInputBuffer, szToken, MAX_TOKEN - 1);
        
        iScanfCnt = sscanf(szInputBuffer, "%ld %d %d %d %d"
            , &event.widget.lWidgetNr
            , &event.widget.iStep1tu
            , &event.widget.iStep2tu
            , &event.widget.iArrivalDelta
            , &event.widget.iWhichServer);
        
        // If the scan count is not correct run ErrExit
        if (iScanfCnt < 5)
        {
            ErrExit(ERR_BAD_INPUT, "Only Received %d Successful Values\n", iScanfCnt);
            continue;
        }
        
        // Set the event type to ARRIVE
        // Change the event time to the current time
        event.iEventType = EVT_ARRIVE;
        event.iTime = iTime;

        // Insert both events into the linked list and change the next event time
        iTime += iArrivalDelta;
        insertOrderedLL(list, event);
    }

    return p;
    fclose(pFilePeople);
}
