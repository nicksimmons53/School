/**************************************************************************************************
p4srz888.c by Nicholas Simmons
Purpose:
    This program will compare the differences in time units and examine the efficiency of adding 
    another server to handle customers in different queues by using two servers and two queues. The
    program will have an arrival that is inserted into a linked list and is then inserted into a 
    queue, once the server becomes available it will then leave the queue and arrive at the server.
    When in the server, it will wait the necessary time and then be reinserted as a COMPLETION. The
    customer is then queued up for the next server and the process repeats until that is done, the 
    customer then leaves the system adding to the total count and waiting time for examination and
    comparison.
Command Parameters:
    p4 -v widgetFile
Input:
    The program will read in output from the pFilePeople, which contains widget name, step 1 time 
    units, step2 time units, an arrival time, and which server it is to be pushed into. 
Results:
    The program will print out a simulation of arrivals, entering queues, seizing servers, leaving
    queues, leaving servers, and leaving systems in chronological order. It will then print average
    wait times in each queue, and average wait time overall. 
Returns:
    It will return 0 if completed correctly. 
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
void getPeopleData(Simulation sim);
void freeNode(LinkedList list, NodeLL *pNode);
void arrival(Event value, Widget *widget);
void queueUp(Simulation sim, Queue queue, Widget *widget);
void seize(Simulation sim, Queue queue, Server server);
void releaseW(Simulation sim, Queue queue, Server server, Widget *widget);
void releaseX(Simulation sim, Queue queue, Server server, Widget *widget);
void leaveSystem(Simulation sim, Widget *widget);
void processCommandSwitches(int argc, char *argv[ ], Simulation sim);

// MAIN FUNCTION FOR THE PROGRAM
//
int main(int argc, char *argv[ ])
{
    // Allocate the memory for the sim structure and eventList
    Simulation sim;
    sim = (Simulation)malloc(sizeof(SimulationImp));
    sim->eventList = newLinkedList( );
    
    // Sort the linked list by iEventTime
    // sort(sim->eventList);
    getPeopleData(sim);
    processCommandSwitches(argc, argv, sim);

    // Run the simulation
    runSimulation(sim);
    
    // Free the memory associated with the simulation struct and LL
    free(sim);
    free(sim->eventList);
    return 0;
}

/************************************** runSimulation *********************************************
void runSimulation(Simulation simulation)

Purpose:        This function will loop through the linked list events, which are inserted and 
                sorted according to the iEventTime. The function then sets the sim->iClock to the
                current time of the event. 
Parameters:     Simulation sim      The sim struct contains the linked list that is being iterated
                                    through and the iClock that is being advanced.
Notes:          The linked list is iterated through using removeLL. As long as removeLL is 
                returning true, the function will continue until termination. 
Returns:        None.
**************************************************************************************************/
void runSimulation(Simulation sim)
{
    // Allocate memory for the servers, give a name, set them to NOT busy
    Server serverX = malloc(sizeof(serverX));
    strcpy(serverX->szServerName, "Server X");
    serverX->bBusy = FALSE;

    Server serverW = malloc(sizeof(serverW));
    strcpy(serverW->szServerName, "Server W");
    serverW->bBusy = FALSE;
    
    // Allocate memory for the queues
    Queue queueX = newQueue("Queue X");
    Queue queueW = newQueue("Queue W");
    
    // Create an empty event struct for the removal return
    Event event;

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
            case EVT_ARRIVAL:
            {
                // Set the event to arrival
                arrival(event, &event.widget);
                // Insert the node into a queue
                queueUp(sim, queueX, &event.widget);
                // If the server is not busy, seize the server
                seize(sim, queueX, serverX);
                break;
            }

            case EVT_SERVERX_COMPLETE:
            {
                // Release from the server once completed
                releaseX(sim, queueX, serverX, &event.widget);
                // Reinsert into the next queue
                queueUp(sim, queueW, &event.widget);
                // Seize the next server if not busy
                seize(sim, queueW, serverW);
                break;
            }

            case EVT_SERVERW_COMPLETE:
            {
                // Release from the server once completed
                releaseW(sim, queueW, serverW, &event.widget);
                // Leave the system and change the queue stats
                leaveSystem(sim, &event.widget);
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
    printf("\n");
    
    // Statistic variables
    double iAverageX = (double) queueX->lQueueWaitSum / queueX->lQueueWidgetTotalCount;
    double iAverageW = (double) queueW->lQueueWaitSum / queueW->lQueueWidgetTotalCount;
    double iTotalAverage = (double) sim->lSystemTimeSum / sim->lWidgetCount;

    printf("Average Queue Time for Server X = %5.2f\n", iAverageX);
    printf("Average Queue Time for Server W = %5.2f\n", iAverageW);
    printf("Average Time in System = %5.2f\n", iTotalAverage);

    // Free the memory associated with the queues and servers
    free(serverX);
    free(serverW);
    free(queueX);
    free(queueW);
}

/************************************** arrival ***************************************************
void arrival(Event value, Simulation sim)

Purpose:        Print a message of arrival and set the current time as the nodes iArrivalTime.
Parameters:     Event       value       Current event the simulation is working on
                Widget      *widget     Current widget the simulation is working on
Notes:          None.
Returns:        None.
**************************************************************************************************/
void arrival(Event value, Widget *widget)
{
    char arrived[26] = "Arrived";
    widget->iArrivalTime = value.iTime;

    printf("%4d %ld %-26s\n"
        , widget->iArrivalTime
        , widget->lWidgetNr
        , arrived);
}

/************************************** queueUp ***************************************************
void queueUp(Simulation sim, Queue queue, Widget widget)

Purpose:        Inserts the current widget into an element, it then inserts that element into the 
                correct queue and prints the information associated with the queueUp. Declares
                an arrival time in the queue for that particular widget.
Paramters:      Simulation      sim         The simulation struct that contains the info
                Queue           queue       The particular queue that is being inserted
                Widget          *widget     Curren widget the simulation is working on
Notes:          None.
Returns:        None.
**************************************************************************************************/
void queueUp(Simulation sim, Queue queue, Widget *widget)
{
    // Declare a QElement struct for insertQ
    QElement element;
    char enterX[26] = "Enter Queue X";
    char enterW[26] = "Enter Queue W";
    sim->bVerbose = FALSE;

    // If queueX, assign the widget info to the element struct for insertion
    if (strcmp(queue->szQName, "Queue X") == 0)
    {
        sim->bVerbose = TRUE;
        element.iEnterQTime = sim->iClock;
        element.widget.lWidgetNr = widget->lWidgetNr;
        element.widget.iStep1tu = widget->iStep1tu;
        element.widget.iStep2tu = widget->iStep2tu;
        element.widget.iArrivalTime = widget->iArrivalTime;
        element.widget.iWhichServer = widget->iWhichServer;

        insertQ(queue, element);

        if (sim->bVerbose = TRUE)
        {
            printf("%4d %ld %-26s\n"
                , sim->iClock
                , element.widget.lWidgetNr
                , enterX);
        }
    }
    
    // If queueW, assign the widget info to the element struct for insertion
    if (strcmp(queue->szQName, "Queue W") == 0)
    {
        sim->bVerbose = TRUE;
        element.iEnterQTime = sim->iClock;
        element.widget.lWidgetNr = widget->lWidgetNr;
        element.widget.iStep1tu = widget->iStep1tu;
        element.widget.iStep2tu = widget->iStep2tu;
        element.widget.iArrivalTime = widget->iArrivalTime;
        element.widget.iWhichServer = widget->iWhichServer;

        insertQ(queue, element);

        if (sim->bVerbose = TRUE)
        {
            printf("%4d %ld %-26s\n"
                , sim->iClock
                , element.widget.lWidgetNr
                , enterW);
        }
    }
}

/************************************** seize *****************************************************
void seize(Simulation sim, Queue queue)

Purpose:        This function removes the first node from the queue and seizes the server for the 
                node as long as the server is not busy. It then reinserts that node back into the 
                linked list and sets the event type as server complete. It also changes the stats
                associated with waiting time and the total count of widgets in the queue.
Parameters:     Simulation      sim         Simulation struct that is currently being run
                Queue           queue       Queue that is being pulled from
                Server          server      Server that is being inserted to
Notes:          None.
Returns:        None.
**************************************************************************************************/
void seize(Simulation sim, Queue queue, Server server)
{
    // Element struct and waiting time variable
    QElement element;
    Event event;
    sim->bVerbose = FALSE;
    int iTimeWaited;
    
    // Only execute as long as the server is not busy
    if (server->bBusy == FALSE)
    {
        // If you can remove from the queue do so
        if (removeQ(queue, &element))
        {
            // Set the time waited to the clock minus the enter time
            iTimeWaited = sim->iClock - element.iEnterQTime;
            sim->bVerbose = TRUE;

            // Print the leaving of the queue and seizing of the server
            if (sim->bVerbose == TRUE)
            {
                printf("%4d %ld Leave %8s, waited %-4d\n"
                    , sim->iClock
                    , element.widget.lWidgetNr
                    , queue->szQName
                    , iTimeWaited);

                printf("%4d %ld Seized %-12s\n"
                    , sim->iClock
                    , element.widget.lWidgetNr
                    , server->szServerName);
            }
            
            // Add the wait time to the total wait sum
            // Add to the total number of widgets that were in the queue
            queue->lQueueWaitSum += iTimeWaited;
            queue->lQueueWidgetTotalCount++;
            
            // Set the server to busy
            // Copy the element widget to the event widget to change event variables
            server->bBusy = TRUE;
            event.widget = element.widget;
            
            // Depending on which server, change the event type and next event time
            if (strcmp(server->szServerName, "Server X") == 0)
            {
                event.iEventType = EVT_SERVERX_COMPLETE;
                event.iTime = sim->iClock + element.widget.iStep1tu;
            }

            if (strcmp(server->szServerName, "Server W") == 0)
            {
                event.iEventType = EVT_SERVERW_COMPLETE;
                event.iTime = sim->iClock + element.widget.iStep2tu;
            }
            
            // Insert the node back into the simulation LL
            insertOrderedLL(sim->eventList, event);
        }
    }
}

/************************************** releaseW **************************************************
void releaseX(Simulation sim, Queue queue, Server server, Widget *widget)

Purpose:        Releases the node from the server when it is done processing. It then tries to 
                reseize the server to immediately pull from the queue to keep the server busy.
Parameters:     Simulation      sim         Simulation struct that is currently running
                Queue           queue       Same queue that was being pulled from
                Server          server      Server that is being released
                Widget          *widget     Current widget that is running
Notes:          None.
Returns:        None.
*************************************************************************************************/
void releaseX(Simulation sim, Queue queue, Server server, Widget *widget)
{
    char releaseX[26] = "Released Server X";

    // Set the server to NOT busy thus releasing it
    server->bBusy = FALSE;
    sim->bVerbose = TRUE;

    if (sim->bVerbose == TRUE)
        printf("%4d %ld %-26s\n"
            , sim->iClock
            , widget->lWidgetNr
            , releaseX);
    
    // Try to seize the same server for the next node
    // Keeping it busy
    seize(sim, queue, server);
}

/************************************** releaseX **************************************************
void releaseX(Simulation sim, Queue queue, Server server, Widget *widget)

Purpose:        Releases the node from the server when it is done processing. It then tries to 
                reseize the server to immediately pull from the queue to keep the server busy.
Paramters:      Simulation      sim         Simulation struct that is currently running
                Queue           queue       Same queue that was being pulled from
                Server          server      Server that is being released
                Widget          *widget     Current widget that is running
Notes:          None.
Returns:        None.
**************************************************************************************************/
void releaseW(Simulation sim, Queue queue, Server server, Widget *widget)
{
    char releaseW[26] = "Released Server W";

    // Set the server to not busy thus releasing it
    server->bBusy = FALSE;
    sim->bVerbose = TRUE;

    if (sim->bVerbose == TRUE)
        printf("%4d %ld %-26s\n"
            , sim->iClock
            , widget->lWidgetNr
            , releaseW);
    
    // Try to seize the same server for the next node
    // Keeping it busy
    seize(sim, queue, server);
}

/************************************** leaveSystem ***********************************************
void leaveSystem(Simulation sim, Widget *widget)

Purpose:        Prints a message stating that the node is leaving the system, updates the stats
                associated with the sim struct.
Parameters:     Simulation      sim         Sim struct that is currently being run
                Widget          *widget     Current widget that is leaving
Notes:          None.
Returns:        None.
**************************************************************************************************/
void leaveSystem(Simulation sim, Widget *widget)
{
    sim->lSystemTimeSum += (widget->iStep1tu + widget->iStep2tu);
    printf("%4d %ld Exit System, in system %4d\n"
        , sim->iClock
        , widget->lWidgetNr
        , widget->iStep1tu + widget->iStep2tu);
}

/************************************** removeQ ***************************************************
int removeQ(Queue queue, QElement *pFromQElement)

Purpose:        Removes a node from the front of the list and returns the element (via the 
                parameter list). 
Parameter:      Queue       queue               Queue that contains the element that is being 
                                                removed
                QElement    *pFromQElement      Element that is being returned back to the call
Notes:          This function must also free the memory associated with the node.
Returns:        int     FALSE       If nothing is in the queue to be removed
                int     TRUE        If there is an element to be returned
**************************************************************************************************/
int removeQ(Queue queue, QElement *pFromQElement)
{
    // Node that will be removed from the queue
    NodeQ *pRemove;
    
    // If the queue is empty return FALSE
    if (queue->pHead == NULL)
        return FALSE;
    
    // Assign the beginning of the queue
    // to be removed
    pRemove = queue->pHead;
    *pFromQElement = pRemove->element;
    queue->pHead = pRemove->pNext;

    if (queue->pHead == NULL)
        queue->pFoot = NULL;
    
    // Free the memory associated with the NodeQ
    free(pRemove);
    
    // Return TRUE if the node is removed properly
    return TRUE;
}

/************************************** insertQ ***************************************************
void insertQ(Queue queue, QElement element)

Purpose:        This function inserts a node at the foot of the list when the queue is not empty.
                Otherwise, it inserts at the head postion.
Parameters:     Queue       queue       Queue that is being inserted into
                QElement    element     Element that is associated with the node that is inserted
Notes:          Must allocate memory for the new node.
Returns:        None.
**************************************************************************************************/
void insertQ(Queue queue, QElement element)
{
    NodeQ *pNew;
    pNew = allocNodeQ(queue, element);

    if (queue->pFoot == NULL)
        queue->pHead = pNew;
    else 
        queue->pFoot->pNext = pNew;

    queue->pFoot = pNew; 
}

/************************************** allocNodeQ ************************************************
NodeQ *allocNodeQ(Queue queue, QElement value)

Purpose:        Allocates memory for a new node that is going to be inserted. If there is no 
                memory to seize, the function calls ErrExit. Otherwise it allocates the memory
                and associates the QElement value with the node.
Parameters:     Queue       queue       Queue that is going to contain the allocated memory
                QElement    value       QElement that is associated with the new memory
Notes:          None.
Returns:        NodeQ       pNew        The node that memory was allocated for
**************************************************************************************************/
NodeQ *allocNodeQ(Queue queue, QElement value)
{
    NodeQ *pNew;
    pNew = (NodeQ *)malloc(sizeof(NodeQ));
    
    // If there is no memory to be allocated, exit
    if (pNew == NULL)
        ErrExit(ERR_ALGORITHM, "No Available Memory for a Queue\n");
    
    // Assign a value to the element and set pNext to NULL
    pNew->element = value;
    pNew->pNext = NULL;
    
    // Return the new node
    return pNew;
}

/************************************** newQueue **************************************************
Queue newQueue(char szQueueNm[ ])

Purpose:        Allocates memory for the actual Queue struct. Sets the head and foot node to NULL
                and copies the name from the parameter list to the variable queue->szQueueNm.
Parameters:     char    szQueueNm[ ]        Character string for the queue name
Notes:          None.
Returns:        Queue       queue       Struct that has now been allocated for.
**************************************************************************************************/
Queue newQueue(char szQueueNm[ ])
{
    // Allocate memory for a new queue
    Queue queue = (Queue)malloc(sizeof(QueueImp));

    // Set the first and last node in the queue to NULL
    queue->pHead = NULL;
    queue->pFoot = NULL;

    // Set the name of the queue to the szQueueNm
    strcpy(queue->szQName, szQueueNm);
    
    // Return the malloced queue
    return queue;
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
void getPeopleData(Simulation sim)
{
    char szInputBuffer[MAX_LINE_SIZE + 1];
    FILE *pFilePeople;
    int iScanfCnt;
    Event event;
    char szToken[MAX_TOKEN];
    int iTime = 0;
    
    // Open the file for a read only
    pFilePeople = fopen("p4Input.txt", "r");
    
    if (pFilePeople == NULL)
        printf("Error: p4Input.txt could not be opened\n");
    
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
            , &event.widget.iArrivalTime
            , &event.widget.iWhichServer);
        
        // If the scan count is not correct run ErrExit
        if (iScanfCnt < 5)
        {
            ErrExit(ERR_BAD_INPUT, "Only Received %d Successful Values\n", iScanfCnt);
            continue;
        }
        
        // Set the event type to ARRIVE
        // Change the event time to the current time
        event.iEventType = EVT_ARRIVAL;
        event.iTime = iTime;

        // Insert both events into the linked list and change the next event time
        iTime += event.widget.iArrivalTime;
        sim->lWidgetCount++;

        insertOrderedLL(sim->eventList, event);
    }

    fclose(pFilePeople);
}

/************************************** processCommandSwitches ************************************
void processCommandSwitches(int argc, char *argv[ ], Simulation sim)

Purpose:        Checks the syntax of command line arguments and returns the filenames. If any 
                switches are unkown, it exits with an error.
Parameters:     int             argc            count of command line arguments
                char            *argv[ ]        array of command line arguments
                Simulation      sim             simulation struct that is being changed
Notes:          If a -v switch is passed, the variable sim->bVerbose is changed to TRUE and then
                prints the messages in a chronological order.
Returns:        None.
**************************************************************************************************/
void processCommandSwitches(int argc, char *argv[ ], Simulation sim)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);

        switch(argv[i][1])
        {
            case 'v':
                sim->bVerbose = TRUE;
        }
    }
}
