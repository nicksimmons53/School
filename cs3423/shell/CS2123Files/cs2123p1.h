/**********************************************************************
cs2123p1.h
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
Notes:
   
**********************************************************************/
/*** constants ***/
// Maximum constants
#define MAX_STACK_ELEM 20       // Maximum number of elements in the stack array
#define MAX_TOKEN 50            // Maximum number of actual characters for a token
#define MAX_OUT_ITEM 50         // Maximum number of PostfixOut items

// Error constants (program exit values)
#define ERR_STACK_USAGE    901
#define ERR_OUT_OVERFLOW   902
#define ERR_ALGORITHM      903

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

// Token typedef used for operators, operands, and parentheses
typedef char Token[MAX_TOKEN + 1];

// Element typedef used for Element values placed in the stack or out array
typedef struct
{
    Token szToken;      // Could be a variable, numeric constant, operator,
                        // left parenthesis or right parenthesis
    int iCategory;      // A value used to make it easier to handle
                        // different cases for the types of tokens
    int iPrecedence;    // An integer representing the operator 
                        // precedence.  Higher values imply 
                        // higher precedence.
} Element;

// StackImp typedef defines how we implement a stack using an array
typedef struct
{
    int iCount;  // number of elements in stack.  0 is empty 
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

// Stack functions

void push(Stack stack, Element value);
Element pop(Stack stack);
int isEmpty(Stack stack);
Stack newStack();
void freeStack(Stack stack);
Element topElement(Stack stack);

// Conversion to Postfix functions that Larry provided
void categorize(Element *pElement);
PostfixOut newPostfixOut();
void addPostfixOut(PostfixOut postfixOut, Element element);
void printPostfixOut(PostfixOut postfixOut);

// Conversion to Postfix functions that each student must implement
int convertToPostfix(char *pszInfix, PostfixOut postfixOut);

// Utility routines
void ErrExit(int iexitRC, char szFmt[], ...);
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize);
