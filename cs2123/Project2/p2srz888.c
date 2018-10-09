/*********************************************************************************************
p2srz888.c by Nicholas Simmons
Purpose:
    This program reads in data from a file and converts the expressions in the file according
    to the precedence assigned to the data. Converts the expressions from infix to postfix. 
    The program then evaluates the expressions based on the token being read. 

Command Parameters:
    n/a

Input:
    A file stream that includes RECIT, PROF, and DEPT. Terminated by EOF or one of the other 
    attributes.

Results:
    For each attribute, the program will convert the expression from infix to postfix 
    according the precedence of the the expression. It will then print the conversion. 

Returns:

Notes:

*********************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS 1 

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p2.h"

/**************************** convertToPostfix ***********************************************
int convertToPostfix(char *pszInfix, PostfixOut out)

Purpose:
    Converts the expressions from the data to Postfix from Infix according to the tokens.

Parameters:
    I char *pszInfix    Pointer to the address of the Infix expression.
    I Out  out          Previous token(s) that was pulled out of the stack. 

Notes:

Return Values:
    0   Conversion completed successfully.
*********************************************************************************************/

int convertToPostfix(char *pszInfix, PostfixOut out)
{
    Element element;
    Element popped;
    Stack stack = newStack( );
    pszInfix = getToken(pszInfix, element.szToken, MAX_TOKEN+1);

    while (pszInfix != NULL)
    {
        categorize(&element);
        
        // Switch statement for the different tokens
        switch (element.iCategory)
        {
            // Case statement for the operands
            case CAT_OPERAND:
            {
                addPostfixOut(out, element);
                break;
            }

            // Case statement for the operators and their precedence
            case CAT_OPERATOR:
            {
                while (!isEmpty(stack))
                {
                    if (element.iPrecedence > topElement(stack).iPrecedence)
                        break;

                    else 
                    {
                        popped = pop(stack);
                        addPostfixOut(out, popped);
                    }
                }

                push(stack, element);
                break;
            }
            
            // Case statement for a right parenthesis
            case CAT_RPAREN:
            {
                while (!isEmpty(stack))
                {
                    popped = pop(stack);

                    if (popped.iCategory == CAT_LPAREN)
                    {
                        break;
                    }

                    addPostfixOut(out, popped);
                }

                if (popped.iCategory != CAT_LPAREN)
                    return WARN_MISSING_LPAREN;

                break;
            }
            
            // Case statement for a left parenthesis
            case CAT_LPAREN:
            {
                push(stack, element);
                break;
            }
        }
        
        // Read in the address of the next token
        pszInfix = getToken(pszInfix, element.szToken, MAX_TOKEN);
    }
    
    // If the stack is not empty at the end of the input
    // pop the remaining tokens and add to Out
    while (!isEmpty(stack))
    {
        popped = pop(stack);
        if (popped.iCategory == CAT_LPAREN)
            return WARN_MISSING_RPAREN;
        addPostfixOut(out, popped);
    }

    // If the stack is empty at the end of the evaluation
    // free the memory location of the stack
    if (isEmpty(stack))
        freeStack(stack);


    // Return for a properly completed statement
    return 0;
}

/**************************** exaluatePostfix ************************************************
void evaluatePostfix(PostfixOut out, Course courseM[ ], int iNumCourse)

Purpose:    Evaluates the postfix expression located in out. Iterates through the courseM
            array and when it finds a satisfying value, it will set the value to true.

Parameters: PostfixOut  out         postfix expression from the stack evaluation
            Course      courseM[ ]  array of the courses
            int         iNumCourse  number of course in the array

Notes:

Return:     void
*********************************************************************************************/

void evaluatePostfix(PostfixOut out, Course courseM[ ], int iNumCourse, QueryResult resultM[ ])
{
    // general iteration value
    int i, j;
    
    // new stack structure
    Stack stack = newStack( );

    // Element structures
    Element postElem;
    Element elementResult;
    Element evalElem;
    Element rightOperand;
    Element leftOperand;

    Attr tempAttr;

    // iterate through the courseM array
    for (i = 0; i < iNumCourse; i++)
    {
        for (j = 0; j < out->iPostfixOutCount; j++)
        {
            postElem = out->postfixOutM[j];

            switch (postElem.iCategory)
            {
                // if the case is an operand push it 
                case CAT_OPERAND:
                {
                    push(stack, postElem);
                    break;
                }
                
                // if the case is an operator evaluate the operator and push the result
                case CAT_OPERATOR:
                {
                    strcpy(evalElem.szBoolean, "Boolean Result of ");
                    strcat(evalElem.szBoolean, postElem.szToken);

                    rightOperand = pop(stack);
                    leftOperand = pop(stack);

                    strcpy(tempAttr.szAttrType, leftOperand.szToken);
                    strcpy(tempAttr.szAttrValue, rightOperand.szToken);

                    if (strcmp(postElem.szToken, "NEVER") == 0)
                        elementResult.bInclude = never(&courseM[i], tempAttr);

                    if (strcmp(postElem.szToken, "ONLY") == 0)
                        elementResult.bInclude = only(&courseM[i], tempAttr);

                    if (strcmp(postElem.szToken, "AND") == 0)
                        elementResult.bInclude = and();

                    push(stack, elementResult);
                    
                    break;
                }
            }
       }

       if (elementResult.bInclude == TRUE)
            resultM[i] = TRUE;
    }
    // if the stack is empty free the memory
    if (isEmpty(stack))
        freeStack(stack);
}

/**************************** atLeastOne *****************************************************
int atLeastOne(Course *pCourse, Attr attr)

Purpose:    Checks the input value for a matching value and returns TRUE or FALSE depending on 
            the outcome.

Paramaters: Course      *pCourse    pointer to the address of the course in the array
            Attr        attr        value that is being checked through the conditions

Notes:

Return:     1   FALSE
            0   TRUE
*********************************************************************************************/

int atLeastOne(Course *pCourse, Attr attr)
{
}

/**************************** only ***********************************************************
int only(Course *pCourse, Attr attr)

Purpose:    Iterates through every course attribute matching the correct course value. If both
            types match, then the value must be the correct matching value. If it does not 
            match, FALSE is returned.

Parameters: Course      *pCourse    pointer to the address of the course in the array
            Attr        attr        value that is being checked through the conditions

Notes:

Return:     0   TRUE
            1   FALSE
*********************************************************************************************/

int only(Course *pCourse, Attr attr)
{
    int i;

    for (i = 0; i < pCourse->iNumberOfAttrs; i++)
    {
        if (strcmp(pCourse->attrM[i].szAttrType, attr.szAttrType) == 0 &&
            strcmp(pCourse->attrM[i].szAttrValue, attr.szAttrValue) == 0)
            return TRUE;
    }

    return FALSE;
}

/**************************** and ************************************************************
int and(Stack stack)

Purpose:
Parameters:
Notes:
Return:
*********************************************************************************************/
int and(Stack stack)
{
}

/**************************** or *************************************************************
int or(Stack stack)

Purpose:
Parameters:
Notes:
Return:
*********************************************************************************************/
int or(Stack stack)
{
}

/**************************** printCourseData ************************************************
void printCourseData(Course courseM[ ], int iNumCourse)

Purpose:    Print the course data for the entire courseM array and puts it into segregated 
            columns and rows.

Parameters: Course   courseM[ ]       array of the courses
            int      iNumCourse       number of courses in the array

Notes:

Return:     void
*********************************************************************************************/

void printCourseData(Course courseM[ ], int iNumCourse)
{
    int i;
    int j;
    
    printf(" %-7s %-20s\n", "ID", "Course Name");
    printf(" \t\t\t %-10s %-12s\n", "Attr", "Value");

    // Iterate through all of the courses 
    for (i = 0; i < iNumCourse; i++)
    {
        printf(" %-7s %-20s\n", courseM[i].szCourseId, courseM[i].szCourseName);
        // Iterate through all of the course attributes
        for (j = 0; j < courseM[i].iNumberOfAttrs; j++)
        {
            // Print each course and its attributes
            printf(" \t\t\t %-10s %-20s\n"
                , courseM[i].attrM[j].szAttrType
                , courseM[i].attrM[j].szAttrValue);
        }
    }
}


