// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p1.h"
#define debug 0
// my prototypes 

// conversion to postfix

int convertToPostfix(char *pszInfix, PostfixOut out)
{
    Element element;
    Stack stack = newStack();
    int rc = 0;
    int bExpectOperand = TRUE;

    pszInfix = getToken(pszInfix, element.szToken, MAX_TOKEN);
    while (pszInfix != NULL)
    {
        categorize(&element);
        switch (element.iCategory)
        {
        case CAT_LPAREN:
            printf("left paren\n");
            break;
        case CAT_RPAREN:
            printf("right paren\n");
            break;
        case CAT_OPERATOR:
            printf("operator\n");
            break;
        default:  // it must be an operand
            printf("operator\n");
            break;
        }
        pszInfix = getToken(pszInfix, element.szToken, MAX_TOKEN);
    }
    // If still expecting an operand, it is an error
    if (rc == 0 && bExpectOperand)
    {
        if (debug)
            printf("Expected operand at end of expression\n");
        rc = WARN_MISSING_OPERAND;
    }
    // empty the stack
    while (rc == 0 && !isEmpty(stack))
    {
        element = pop(stack);
    }
    free(stack);
    return rc;
}
