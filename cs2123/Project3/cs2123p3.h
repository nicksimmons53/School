 /**********************************************************************
cs2123p3.h
Purpose:
    Defines constants:
        max constants
        error constants
        event type constants
        boolean constants
    Defines typedef for
        Token
        Person
        Event (instead of Element)
        For Linked List
            NodeLL
            LinkedListImp
            LinkedList
        For the simulation
            SimulationImp
            Simulation
    Protypes
        Functions provided by student
        Other functions provided by Larry previously (program 2)
        Utility functions provided by Larry previously (program 2)
Notes:

**********************************************************************/

/*** constants ***/
// Maximum constants

#define MAX_TOKEN 50            // Maximum number of actual characters for a token
#define MAX_LINE_SIZE 100       // Maximum number of character per input line

// Error constants (program exit values)
#define ERR_COMMAND_LINE    900    // invalid command line argument
#define ERR_ALGORITHM       903    // Error in algorithm - almost anything else
#define ERR_BAD_INPUT       503    // Bad input 

// Error Messages 
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found"
#define ERR_MISSING_ARGUMENT        "missing argument for"

// Event Constants
#define EVT_ARRIVE          1      // when a person arrives
#define EVT_DEPART          2      // when a person departs the simulation 

// exitUsage control 
#define USAGE_ONLY          0      // user only requested usage information
#define USAGE_ERR           -1     // usage error, show message and usage information

// boolean constants
#define FALSE 0
#define TRUE 1

/*** typedef ***/

// Token typedef used for operators, operands, and parentheses
typedef char Token[MAX_TOKEN + 1];


// Person typedef 
typedef struct
{
    char szName[16];       // Name
    int iDepartUnits;      // time units representing how long he/she stays around
} Person;

// Event typedef (aka Element)
typedef struct
{
    int iEventType;        // The type of event as an integer:
                           //    EVT_ARRIVE - arrival event
                           //    EVT_DEPART - departure event
    int iTime;             // The time the event will occur 
    Person person;         // The person invokved in the event.
} Event;

// NodeLL typedef - these are the nodes in the linked list
typedef struct NodeLL
{
    Event event;
    struct NodeLL *pNext;  // points to next node in the list
} NodeLL;

// typedefs for the ordered link list 
typedef struct
{
    NodeLL *pHead;         // Points to the head of the ordered list
} LinkedListImp;

typedef LinkedListImp *LinkedList;

// typedefs for the Simulation
typedef struct
{
    int iClock;            // clock time
    LinkedList eventList;
} SimulationImp;
typedef SimulationImp *Simulation;

/**********   prototypes ***********/

// linked list functions - you must provide the code for these (see course notes)
int removeLL(LinkedList list, Event  *pValue);
NodeLL *insertOrderedLL(LinkedList list, Event value);
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes);
LinkedList newLinkedList();
NodeLL *allocateNodeLL(LinkedList list, Event value);

// simulation functions - you must provide code for this
void runSimulation(Simulation simulation);

// functions in most programs, but require modifications
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);

// Utility routines provided by Larry (copy from program #2)
void ErrExit(int iexitRC, char szFmt[], ...);
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize);
