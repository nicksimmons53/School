#include <stdio.h>
#include <string.h>
#include "cmdparse.h"

#define DEBUG FALSE

void printCmd(CMD *cmd);

/**
 * Creates Cmd structure corresponding to raw command string.
 * Parameters:
 *  rawCmd - pointer to raw string consisting of, at most, two sub-commands.
 *      May contain "|", "&", "<", ">", and/or ">>"
 *  c = pointer to Cmd structure to parse command into.
 **/
int cmdparse(char *rawCmd, CMD *c) {
    char *token;
    int i;

    c->pipelining = FALSE;
    c->background = FALSE;
    c->redirectIn = FALSE;
    c->redirectOut = FALSE;
    c->redirectAppend = FALSE;
    c->argv1[0] = NULL;
    c->argv2[0] = NULL;
    c->infile = NULL;
    c->outfile = NULL;

    // strip newline if exists
    rawCmd[strcspn(rawCmd, "\n")] = 0;

    // extract first token 
    if ((c->argv1[0] = strtok(rawCmd, " ")) == NULL){
        return(NO_INPUT);
    }

    // extract remaining tokens until a special character is found, indicating end of argv1
    i = 1;
    token = strtok(NULL, " ");
    while(token != NULL && !isSpecialChar(token)){
        c->argv1[i++] = token;
        token = strtok(NULL, " ");
    }
    // set last string to NULL for execvp
    c->argv1[i] = NULL;

    // next token is special character or NULL regardless of existence of argv2
    while (token != NULL){

        if(!strcmp(token, "|")){
            // should not be detected a second time
            if (c->pipelining)
                return(PARSE_ERROR);

            c->pipelining = TRUE;
        
            // second command only exists if pipelining is true
            token = strtok(NULL, " ");
            i = 0;
            while(token != NULL && !isSpecialChar(token)){
                c->argv2[i++] = token;
                token = strtok(NULL, " ");
            }
            c->argv2[i] = NULL;
            if(c->argv2[0] == NULL)
                return(PARSE_ERROR);
            continue; // skip strtok
        }else if(!strcmp(token, "&")){
            // should not be detected a second time
            if (c->background)
                return(PARSE_ERROR);

            c->background = TRUE;

            // no tokens should follow
            if ((token = strtok(NULL, " ")) != NULL)
                return(PARSE_ERROR);
            break;
        }else if(!strcmp(token, "<")){
            // should not be detected a second time
            if (c->redirectIn)
                return(PARSE_ERROR);

            c->redirectIn = TRUE;
            // next token should be file
            if ((c->infile = strtok(NULL, " ")) == NULL)
                return(PARSE_ERROR);
        }else if(!strcmp(token, ">") || !strcmp(token, ">>")){
            // should not be detected a second time
            if (c->redirectOut)
                return(PARSE_ERROR);
            c->redirectOut = TRUE;

            if (!strcmp(token, ">>"))
                c->redirectAppend = TRUE;
            // next token should be file
            if ((c->outfile = strtok(NULL, " ")) == NULL)
                return(PARSE_ERROR);
        }else
            return(PARSE_ERROR);

        token = strtok(NULL, " ");
    }
    if(DEBUG)
        printCmd(c);
    return(0);
}

/**
 * Returns 1 if the string matches a command terminator:
 * "|", "&", "<", ">", ">>"
 **/
int isSpecialChar(char *str){
    if(!strcmp(str, "|") || !strcmp(str, "&") || !strcmp(str, "<") || !strcmp(str, ">")
            || !strcmp(str, ">>"))
        return(1);
    return(0);
}

/**
 * Prints contents of Cmd struct.
 **/
void printCmd(CMD *cmd){
    printf("--------BEGIN PARSE RESULTS--------\n");
    printf("argv1: ");
    int i = 0;
    while(cmd->argv1[i] != NULL)
        printf("%s ", cmd->argv1[i++]);
    printf("\n");
    
    if(cmd->argv2[0] != NULL){
        printf("argv2: ");
        i = 0;
        while(cmd->argv2[i] != NULL)
            printf("%s ", cmd->argv2[i++]);
        printf("\n");
    }

    if(cmd->pipelining)
        printf("pipelining detected\n");
    if(cmd->background)
        printf("background detected\n");
    if(cmd->redirectIn)
        printf("input redirection detected\n");
    if(cmd->infile != NULL)
        printf("infile: %s\n", cmd->infile);
    if(cmd->redirectOut)
        printf("output redirection detected\n");
    if(cmd->redirectAppend)
        printf("append detected\n");
    if(cmd->outfile != NULL)
        printf("outfile: %s\n", cmd->outfile);
    printf("---------END PARSE RESULTS---------\n");
}
