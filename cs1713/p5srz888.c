/**********************************************************************
p5srz888.c by Nicholas Simmons
Purpose:
    This program reads team information and a command file.   It 
    processes the commands against the team information.
    This is the file containing functions written by the students.
Input:
    Team    same as Programming Assignment #2 although there is different data.

    Command This is different from the previous assignment.  The file contains  text in the form of commands.
        GAME RESULT szTeamId1 szTeamId2 iScore1 iScore2
            This should use processGame to process this 
        GAME FIX szTeamId1 szTeamId2 iOldScoreTeam1 iOldScoreTeam2 iNewScore1 iNewScore2
            if the winner changes:
               - decrement the old winner's iWin the old loser's iLoss
               - increment the new winner's iWin the new loser's iLoss
            Note that the teams must exist in the team array.  If not, show a warning.
        TEAM PAID szTeamId dAmount
             Add the amount to this team's dPaidAmount.  The team must exist in the array.
        TEAM SHOW szTeamId
             Show all the team information for this one team.  The team must exist in the array.      
Notes:
    This file contains functions:
       printTeams - prints the team array's contents
       processGameCommand - processes the subcommands associated with the GAME command
       processteamCommand - processes the subcommands associated with the TEAM command
       searchteams - using a binary search, it finds the specified team in the teamM array.
       processGame - processes a game data record, checking the data and changing
                     the wins/losses for teams
       processGameFix - processes a fix to game scores, checking the data and possibly
                     changing the wins/losses for teams.
       searchT - searches the binary tree and returns the correct location
       insertT - inserts a node into the tree into the correct spot alphabetically
       prettyPrintT - prints the tree in an easy to read form
       printInOrder - prints and sorts the tree into the correct form alphabetically 
**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p5.h"

/******************** printTeams *****************************
   void printTeams(char szHeading[], NodeT *pRoot)
Purpose:
    Prints team information. 
Parameters:
    char szHeading[]  string describing why the teams are printed
                      (e.g., "Original Team Information")
    NodeT *pRoot      pointer to the start of the binary tree
Notes:
    Prints two lines of text per team.
**************************************************************************/
void printTeams(char szHeading[], NodeT *pRoot)
{
    NodeT *p;          // local counter for NodeT to the tree

    // Print the heading
    printf("%s\n", szHeading);
    printf("  %-6s %-12s %4s %4s  %-8s  %-8s\n"
        , "Id", "Team Name", "Wins", "Loss", "Fee Amt", "Paid Amt");
    printf("\t\t  %-20s %-13s %s\n"
        , "Contact Name", "Phone", "Email");

    // calls printInOrder to print the team info and iterate
    printInOrder(pRoot);
}


/********************processTeamCommand *****************************
    void processTeamCommand(NodeT **ppRoot, char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
        TEAM PAID szTeamId dAmount
             Add the amount to this team's dPaidAmount.  The team must exist in the array.
        TEAM SHOW szTeamId
             Show all the team information for this one team.  The team must exist in the array
        TEAM NEW  szTeamId iWins iLosses dFee dPaid
             Add the new team with the information to the tree.
        TEAM CONTACT szTeamId szTeamName szEmailAddr szPhone szContactname
             Add the contact information to the correct team.
        TEAM PPRINT 
             Print the team id's in a binary tree using prettyPrintT.
Parameters:
        NodeT **ppRoot                pointer to the beginning of the binary tree
    I   char  *pszSubCommand          Should be PAID or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand). 
Notes:

**************************************************************************/
void processTeamCommand(NodeT **ppRoot, char *pszSubCommand, char *pszRemainingInput)
{
    Team team;
    NodeT *p;                    // result of searchT of the node
    int iScanfCnt, iScanfCnt2;
    double dAmount;
    
    // Determine what to do based on the PAID subCommand
    if (strcmp(pszSubCommand, "PAID") == 0)
    {
        // get the szTeamId and dAmount
        iScanfCnt = sscanf(pszRemainingInput, "%6s %lf", team.szTeamId, &dAmount);

        // Check for bad data
        if (iScanfCnt != 2)
            exitError(ERR_TEAM_PAID_DATA, pszRemainingInput);
        
        // subscript of szTeamId location in the binary tree
        p = searchT(*ppRoot, team.szTeamId);

        // if szTeamId is not found exit function
        if (p == NULL)
        {
            printf(" *** team (%s) not found\n", team.szTeamId);
            return;
        }

        // add the paid amount to the team's previous paid amount
        p->team.dPaidAmount += dAmount;
   }
   
   // Determine what to do based on the SHOW subCommand
    else if (strcmp(pszSubCommand, "SHOW") == 0)
    {
        // get the szTeamId
        iScanfCnt = sscanf(pszRemainingInput, "%6s", team.szTeamId);
        
        // Check for bad data
        if (iScanfCnt != 1)
            exitError(ERR_TEAM_SHOW_DATA, pszRemainingInput);
        
        // find the team 
        p = searchT(*ppRoot, team.szTeamId);
        
        // if the team is not found, print warning and leave the command
        if (p == NULL)
        {
            printf( "*** team (%s) not found\n", team.szTeamId);
            printf("\n");
            return;
        }

        // print the team data for SHOW command
        else if (p != NULL)
        {
        printf("  %-6s %-12s %4d %4d  %8.2lf  %8.2lf\n"
            , p->team.szTeamId
            , p->team.szTeamName
            , p->team.iWins
            , p->team.iLosses
            , p->team.dFeeAmount
            , p->team.dPaidAmount);
        printf("\t\t  %-20s %-13s %s\n"
            , p->team.szContactname
            , p->team.szPhone
            , p->team.szEmailAddr);
         }
    }

    //  Determine what to do based on the NEW command
    else if (strcmp(pszSubCommand, "NEW") == 0)
    {
        // get the team info
        iScanfCnt = sscanf(pszRemainingInput, "%6s %d %d %lf %lf"
            , team.szTeamId
            , &team.iWins
            , &team.iLosses
            , &team.dFeeAmount
            , &team.dPaidAmount);

        // Check for bad data
        if (iScanfCnt != 5)
        {
            exitError("bad data for team show command", pszRemainingInput);
        }
        
        // find the team
        p = searchT(*ppRoot, team.szTeamId);

        // Insert the data into the linked list
        if (p != NULL)
        {
            printf(" *** team (%s) already exists", p->team.szTeamId);
            printf("\n");
        }

        else if (p == NULL)
            *ppRoot = insertT(*ppRoot, team);
    }

    // Determine what to do based on the CONTACT command
    else if (strcmp(pszSubCommand, "CONTACT") == 0)
    {
        // get the team info
        iScanfCnt = sscanf(pszRemainingInput, "%6[^,],"
            , team.szTeamId);
        
        // Check for bad data
        if (iScanfCnt != 1)
            exitError(ERR_TEAM_CONTACT_DATA, pszRemainingInput);
        
        // Find the team
        p = searchT(*ppRoot, team.szTeamId);
        
        iScanfCnt2 = sscanf(pszRemainingInput, "%*[^,],%12[^,],%30[^,],%13[^,],%20[^\n]\n"
            , team.szTeamName
            , team.szEmailAddr
            , team.szPhone
            , team.szContactname);

        // Check for bad data
        if (iScanfCnt2 != 4)
            exitError(ERR_TEAM_CONTACT_DATA, pszRemainingInput);

        // Copy the data to the appropriate node
        if (p != NULL)
        {
            strcpy(p->team.szTeamName, team.szTeamName);
            strcpy(p->team.szEmailAddr, team.szEmailAddr);
            strcpy(p->team.szPhone, team.szPhone);
            strcpy(p->team.szContactname, team.szContactname);
        }
        
        // if the team is not detected, print a warning and leave the command
        if (p == NULL)
        {
            printf(" *** team (%s) not found\n", team.szTeamId);
            return;
        }
    }
    
    // Determine what to do based on the PPRINT command 
    else if (strcmp(pszSubCommand, "PPRINT") == 0)
    {
        prettyPrintT(*ppRoot, 0);
    }

    else printf(WARN_TEAM_SUB_COMMAND, pszSubCommand);
}

/********************processGameCommand *****************************
    void processGameCommand(NodeT *pRoot, char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the GAME command:
        GAME RESULT szTeamId1 szTeamId2 iScore1 iScore2
            This should use processGame to process this 
        GAME FIX szTeamId1 szTeamId2 iOldScoreTeam1 iOldScoreTeam2 iNewScore1 iNewScore2
            if the winner changes:
               - decrement the old winner's iWin the old loser's iLoss
               - increment the new winner's iWin the new loser's iLoss
            Note that the teams must exist in the team array.  If not, show a warning.
                
Parameters:
        NodeT *pRoot                  Pointer to the beginning of the tree
    I   char  *pszSubCommand          Should be CHANGE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processGameCommand(NodeT *pRoot, char *pszSubCommand, char *pszRemainingInput)
{
    Game game;
    int iScanfCnt;
    NodeT *i, *j;             // subscripts of team locations in the tree
    int iOldScoreTeam1, iOldScoreTeam2;
    int iNewScore1, iNewScore2;

    // Determine what to do based on the RESULT subCommand
    if (strcmp(pszSubCommand, "RESULT") == 0)
    {
        // scan the file for game data
        iScanfCnt = sscanf(pszRemainingInput, "%7s %7s %d %d"
            , game.szTeamId1
            , game.szTeamId2
            , &game.iScore1
            , &game.iScore2);
        
        // Check data for correct input
        if (iScanfCnt != 4)
        {
            printf("%s", ERR_GAME_DATA);
            exit(-1);
        }
        
        // if the teams are the same print a warning and exit function
        if (strcmp(game.szTeamId1, game.szTeamId2) == 0)
        {
            printf("%s\n", WARN_SAME_TEAM);
            return;
        }
        
        // subscript location of both teams in the tree
        i = searchT(pRoot, game.szTeamId1);
        j = searchT(pRoot, game.szTeamId2);
        
        // if either team are not found print a warning and exit function
        if (i == NULL)
        {
            printf(" *** team(%s) not found\n", game.szTeamId1);
            return;
        }

        if (j == NULL)
        {
            printf(" *** team(%s) not found\n", game.szTeamId2);
            return;
        }

        processGame(pRoot, game);   // call processGame to run data that is being scanned
    }
    
    // Determine what to do based on the FIX subCommand
    if (strcmp(pszSubCommand, "FIX") == 0)
    {
        // scan for the game fix data
        iScanfCnt = sscanf(pszRemainingInput, "%7s %7s %d %d %d %d"
            , game.szTeamId1
            , game.szTeamId2
            , &iOldScoreTeam1
            , &iOldScoreTeam2
            , &game.iScore1
            , &game.iScore2);
        
        // check data for correct input
        if (iScanfCnt != 6)
        {
            printf("%s\n", ERR_GAME_DATA);
            exit(-1);
        }

        // subscript of both teams location in the tree
        i = searchT(pRoot, game.szTeamId1);
        j = searchT(pRoot, game.szTeamId2);

        // If either team is not found the funtion is exited
        // and the wins and losses are not affected
        if (i == NULL)
        {
            printf(" *** team (%s) not found\n", game.szTeamId1);
            return;
        }

        if (j == NULL)
        {
            printf(" *** team (%s) not found\n", game.szTeamId2);
            return;
        }
        
        // Call to processGameFix to process the wins and losses 
        // according to the new scores
        processGameFix(pRoot, game, iOldScoreTeam1, iOldScoreTeam2);
    }
}

/******************* processGame *******************************************
Purpose: 
    Processes the data from the game file and runs for each iteration. Finds
    the team using a recursive search of the tree to increment the wins and losses and for
    general processing.
Parameters:
    NodeT *pRoot        root of the binary tree
    Game game           game struct
***************************************************************************/
void processGame(NodeT *pRoot, Game game)
{
    NodeT *pPrecedes;
    NodeT *p1;
    NodeT *p2;

    // Search the tree for the correct location of the Team
    p1 = searchT(pRoot, game.szTeamId1);
    p2 = searchT(pRoot, game.szTeamId2);
    
    // Analyze the scores of both games
    if (game.iScore1 > game.iScore2)
    { 
        p1->team.iWins++;
        p2->team.iLosses++;
    }

    if (game.iScore1 < game.iScore2)
    {
        p1->team.iLosses++;
        p2->team.iWins++;
    }

    // Error warning if the game was a tie
    if (game.iScore1 == game.iScore2)
        printf("%s\n", WARN_GAME_WAS_TIE);
    
}

/*********************************************************************************************
    void processGameFix(NodeT *pRoot, Game game, int iOldScore1, int iOldScore2)
Purpose:
    Processes the games and corrects the win and loss column according to the GAME FIX command
    that is being called in the processGameCommand function.

Parameters:
    NodeT *pRoot    pointer to the root of the binary tree
    Game game       struct of game file
    int iOldScore1  old score that needs to be fixed
    int iOldScore2  old score that needs to be fixed

Notes:
    If the new scores are in a tie, the winner of the old scores wins will be decremented
    and the losers losses will be decremented to reflect the tie.

*********************************************************************************************/
void processGameFix(NodeT *pRoot, Game game, int iOldScore1, int iOldScore2)
{
    NodeT *p1;
    NodeT *p2;
    int iNewScore1, iNewScore2;
    
    // Locations of both team IDs in the tree
    p1 = searchT(pRoot, game.szTeamId1);
    p2 = searchT(pRoot, game.szTeamId2);
    
    // If the winner is the same, exit the function
    if (iOldScore1 > iOldScore2 && game.iScore1 > game.iScore2)
        return;
    
    // If the winner is the same, exit the function
    if (iOldScore1 < iOldScore2 && game.iScore1 < game.iScore2)
        return;
    
    // if the new scores are in a tie, correctly decrement both teams wins and losses
    // to reflect the tie
    if (iOldScore1 != iOldScore2 && game.iScore1 == game.iScore2)
        printf("%s\n", WARN_GAME_WAS_TIE);
    
    // If the new scores are not equal to the old scores process the new scores
    if (iOldScore1 != game.iScore1 || iOldScore1 != game.iScore2)
    {
        // Correct the win and loss columns if team1 wins
        if (game.iScore1 > game.iScore2)
        {
            p1->team.iWins++;
            p1->team.iLosses--;
            p2->team.iWins--;
            p2->team.iLosses++;
        }
        
        // Correct the win and loss columns if team2 wins
        if (game.iScore1 < game.iScore2)
        {
            p1->team.iWins--;
            p1->team.iLosses++;
            p2->team.iWins++;
            p2->team.iLosses--;
        }
    }
}

/*********************************************************************************************
    NodeT *searchT(NodeT *p, char szMatchTeamId[ ]);
Purpose:        Searches the binary tree recursively, returning the location of the function
                if found. If not found the function returns NULL. 
Parameters:     NodeT *p                    Pointer to the beginning of the binary tree
                char szMatchTeamId[ ]       String of the team ID being searched for
Notes:
*********************************************************************************************/

NodeT *searchT(NodeT *p, char szMatchTeamId[ ])
{
    if (p == NULL)
        return NULL;

    else if (strcmp(p->team.szTeamId, szMatchTeamId) == 0)
        return p;

    else if (strcmp(p->team.szTeamId, szMatchTeamId) > 0)
        return searchT(p->pLeft, szMatchTeamId);

    else if (strcmp(p->team.szTeamId, szMatchTeamId) < 0)
        return searchT(p->pRight, szMatchTeamId);
}

/*********************************************************************************************
    NodeT *insertT(NodeT *p, Team team);
Purpose:        Inserts a node into the proper location in the tree according to the team ID. 
                The function will sort the teams down the correct side of the tree depending
                on the strcmp return value. 
Parameters:     NodeT *p       pointer to the root of the binary tree
                Team team      Team struct that belongs to the node being inserted
Notes:
*********************************************************************************************/

NodeT *insertT(NodeT *p, Team team)
{
    // if node is null, allocate a node for data
    if (p == NULL)
        return allocateNodeT(team);
    
    // if the team already exists return the location
    if (strcmp(p->team.szTeamId, team.szTeamId) == 0)
        return p;
    
    // if the node is less than the root go to the left of the tree
    else if (strcmp(p->team.szTeamId, team.szTeamId) > 0)
    {
        p->pLeft = insertT(p->pLeft, team);
        return p;
    }
    
    // if the node is greater than the root go to the right of the tree
    else if (strcmp(p->team.szTeamId, team.szTeamId) < 0)
    {
        p->pRight = insertT(p->pRight, team);
        return p;
    }
}

/*********************************************************************************************
    void prettyPrintT(NodeT *p, int iIndent);
Purpose:        Recursively prints the binary tree in a correct order based on the alphabetical sorting
                of the nodes. Makes the tree easier to read in printing.
Parameters:     NodeT *p        Pointer to the start of the tree
                int iIndent     Integer for the amount of indented space
Notes:          Called in printTeams to organize the printing structure of the tree.
*********************************************************************************************/

void prettyPrintT(NodeT *p, int iIndent)
{
    int i;          // Local counter for the loop
    
    // if the tree root is NULL exit the function
    if (p == NULL)
        return;
    
    // calls prettyPrintT to correctly print the right side of the tree
    prettyPrintT(p->pRight, iIndent + 1);
    
    // the loop tells where the indention needs to be for the
    // correct node
    for (i = 0; i < iIndent; i++)
    {
        printf("   ");
    }
    
    // prints the team ID
    printf("%6s\n", p->team.szTeamId);
    
    // calls prettyPrintT to print the left side of the tree
    prettyPrintT(p->pLeft, iIndent + 1);
}

/*********************************************************************************************
    void printInOrder(NodeT *p)
Purpose:     Prints all of the nodes in the binary tree recursively and in order according to 
             the tree. If the tree is NULL the function is exited. 
Parameters:  NodeT *p           Pointer to the root of the binary tree 
Notes:
*********************************************************************************************/

void printInOrder(NodeT *p)
{
    // if the root is NULL, nothing to print
    if (p == NULL)
        return;

     printInOrder(p->pLeft);

     printf("  %-6s %-12s %4d %4d  %8.2lf  %8.2lf\n"
        , p->team.szTeamId
        , p->team.szTeamName
        , p->team.iWins
        , p->team.iLosses
        , p->team.dFeeAmount
        , p->team.dPaidAmount);
     printf("\t\t  %-20s %-13s %s\n"
        , p->team.szContactname
        , p->team.szPhone
        , p->team.szEmailAddr);

     printInOrder(p->pRight);
}

