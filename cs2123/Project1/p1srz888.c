/*********************************************************************************************
p1srz888.c by Nicholas Simmons
Purpose:
    This program reads in data from a file and converts the expressions in the file according
    to the precedence assigned to the data. Converts the expressions from infix to postfix. 

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
#include "cs2123p1.h"

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
