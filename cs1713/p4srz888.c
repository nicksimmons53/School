/**********************************************************************
p4srz888.c by your Nicholas Simmons
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
           getTeams - retrieves the teams and places them in an array for the specified file.
           sortTeams - sorts the team array by team ID
           printTeams - prints the team array's contents
       processGameCommand - processes the subcommands associated with the GAME command
       processteamCommand - processes the subcommands associated with the TEAM command
           searchteams - using a binary search, it finds the specified team in the teamM array.
           processGame - processes a game data record, checking the data and changing
                     the wins/losses for teams
       processGameFix - processes a fix to game scores, checking the data and possibly
                     changing the wins/losses for teams.
**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p4.h"

/******************** printTeams *****************************
   void printTeams(char szHeading[], Team teamM[], int iTeamCnt)
Purpose:
    Prints team information. 
Parameters:
    char szHeading[]  string describing why the teams are printed
                      (e.g., "Original Team Information")
    Node *pHead       starting point in the linked list from the node struct 
Notes:
    Prints two lines of text per team.
**************************************************************************/
void printTeams(char szHeading[], Node *pHead)
{
    Node *p;          // local counter for Node 

    // Print the heading
    printf("%s\n", szHeading);
    printf("  %-6s %-12s %4s %4s  %-8s  %-8s\n"
        , "Id", "Team Name", "Wins", "Loss", "Fee Amt", "Paid Amt");
    printf("\t\t  %-20s %-13s %s\n"
        , "Contact Name", "Phone", "Email");

    // iterate through the teamM array and print the contents
    for (p = pHead; p != NULL; p = p->pNext)
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


/********************processTeamCommand *****************************
    void processTeamCommand(Team teamM[], int iTeamCnt
     , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
        TEAM PAID szTeamId dAmount
             Add the amount to this team's dPaidAmount.  The team must exist in the array.
        TEAM SHOW szTeamId
             Show all the team information for this one team.  The team must exist in the array. 
Parameters:
        Node  **ppHead                 Points to the beginning of the team node
    I   char  *pszSubCommand          Should be PAID or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand). 
Notes:

**************************************************************************/
void processTeamCommand(Node **ppHead, char *pszSubCommand, char *pszRemainingInput)
{
    Team team;
    Node *p;                    // result of searchLL of the node
    Node *pPrecedes;            // pointer of the previous node
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
        
        // subscript of szTeamId location in teamM[]
        p = searchLL(*ppHead, team.szTeamId, &pPrecedes);

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
        p = searchLL(*ppHead, team.szTeamId, &pPrecedes);
        
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
        p = searchLL(*ppHead, team.szTeamId, &pPrecedes);

        // Insert the data into the linked list
        if (p != NULL)
        {
            printf(" *** team (%s) already exists", p->team.szTeamId);
            printf("\n");
        }

        else if (p == NULL)
            insertLL(ppHead, team);
    }

    // Determine what to do based on the CONTACT command
    else if (strcmp(pszSubCommand, "CONTACT") == 0)
    {
        // get the team info
        iScanfCnt = sscanf(pszRemainingInput, "%6[^,],"
            , team.szTeamId);
        
        // Find the team
        p = searchLL(*ppHead, team.szTeamId, &pPrecedes);

        iScanfCnt2 = sscanf(pszRemainingInput + 7, "%12[^,],%30[^,],%13[^,],%20[^\n]\n"
            , team.szTeamName
            , team.szEmailAddr
            , team.szPhone
            , team.szContactname);

        // Check for bad data
        if (iScanfCnt != 1)
            exitError(ERR_TEAM_SHOW_DATA, pszRemainingInput);

        if (iScanfCnt2 != 4)
            exitError(ERR_TEAM_SHOW_DATA, pszRemainingInput);

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
            printf(" *** team (%s) does not exist\n", team.szTeamId);
            return;
        }
    }

    else printf(WARN_TEAM_SUB_COMMAND, pszSubCommand);
}

/********************processGameCommand *****************************
    void processGameCommand(Team teamM[], int iTeamCnt
         , char *pszSubCommand, char *pszRemainingInput)
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
    I/O Team teamM[]                  Array of teams
    I   int   iTeamCnt                Number of elements in teamM[]
    I   char  *pszSubCommand          Should be CHANGE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processGameCommand(Node **ppHead, char *pszSubCommand, char *pszRemainingInput)
{
    Game game;
    int iScanfCnt;
    Node *i, *j;             // subscripts of team locations in the array
    int iOldScoreTeam1, iOldScoreTeam2;
    int iNewScore1, iNewScore2;
    Node *pPrecedes;

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
        if(strcmp(game.szTeamId1, game.szTeamId2) == 0)
        {
            printf("%s\n", WARN_SAME_TEAM);
            return;
        }
        
        // subscript location of both teams in teamM[]
        i = searchLL(*ppHead, game.szTeamId1, &pPrecedes);
        j = searchLL(*ppHead, game.szTeamId2, &pPrecedes);
        
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

        processGame(ppHead, game);   // call processGame to run data that is being scanned
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

        // subscript of both teams location in teamM[]
        i = searchLL(*ppHead, game.szTeamId1, &pPrecedes);
        j = searchLL(*ppHead, game.szTeamId2, &pPrecedes);

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
        processGameFix(ppHead, game, iOldScoreTeam1, iOldScoreTeam2);
    }
}

/******************* processGame *******************************************
Purpose: 
    Processes the data from the game file and runs for each iteration. Finds
    the team using a binary search to increment the wins and losses and for
    general processing.
Parameters:
    C   Struct of the game file
    C   Array of the team information
    I   Count of teams entered
***************************************************************************/
void processGame(Node **ppHead, Game game)
{
    Node *pPrecedes;
    Node *p1;
    Node *p2;

    // Binary search for team index
    p1 = searchLL(*ppHead, game.szTeamId1, &pPrecedes);
    p2 = searchLL(*ppHead, game.szTeamId2, &pPrecedes);
    
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
    void processGameFix(Game game, Team teamM[], int iTeamCnt, int iOldScore1, int iOldScore2)
Purpose:
    Processes the games and corrects the win and loss column according to the GAME FIX command
    that is being called in the processGameCommand function.

Parameters:
    Game game       struct of game file
    Team teamM[]    array of teams
    int iTeamCnt    Number of elements in teamM[]
    int iOldScore1  old score that needs to be fixed
    int iOldScore2  old score that needs to be fixed

Notes:
    If the new scores are in a tie, the winner of the old scores wins will be decremented
    and the losers losses will be decremented to reflect the tie.

*********************************************************************************************/
void processGameFix(Node **ppHead, Game game, int iOldScore1, int iOldScore2)
{
    Node *pPrecedes;
    Node *p1;
    Node *p2;
    int iNewScore1, iNewScore2;
    
    // Locations of both team IDs in the array
    p1 = searchLL(*ppHead, game.szTeamId1, &pPrecedes);
    p2 = searchLL(*ppHead, game.szTeamId2, &pPrecedes);
    
    // If the winner is the same, exit the function
    if (iOldScore1 > iOldScore2 && game.iScore1 > game.iScore2)
        return;
    
    // If the winner is the same, exit the function
    if (iOldScore1 < iOldScore2 && game.iScore1 < game.iScore2)
        return;
    
    // if the new scores are in a tie, correctly decrement both teams wins and losses
    // to reflect the tie
    if (iOldScore1 != iOldScore2 && game.iScore1 == game.iScore2)
    {
        if (iOldScore1 > iOldScore2)
        {
            p1->team.iWins--;
            p2->team.iLosses--;
        }

        if (iOldScore1 < iOldScore2)
        {
            p1->team.iLosses--;
            p2->team.iWins--;
        }
    }
    
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
/***********************************************************************************************
    Node *insertLL(Node **ppHead, Team team)
Purpose: 
    Insertion node for team info into the linked list. If the team is found, the function is 
    exited. If the team is not found, the function iterates to the correct spot using searchLL
    and inserts at that correct spot. The function then returns the location of the new Node. 
Parameters:
    **ppHead        Pointer to the pointer of the start of the linked list
    team            Struct of the team information
Notes:
***********************************************************************************************/
Node *insertLL(Node **ppHead, Team team)
{
    Node *pNew;                 // variable for pointer towards new node
    Node *pPrecedes;            // variable for pointer towards previous node

    // check to see if the node already exists or if the
    // return is NULL
    pNew = searchLL(*ppHead, team.szTeamId, &pPrecedes);
    
    // if the return is NULL use pNew as the pointer to the new Node allocation
    if (pNew == NULL)
    {
        pNew = allocateNode(team);
        
        // if the previous node is not NULL iterate to the next node for insertion
        if (pPrecedes != NULL) 
        {
            pNew->pNext = pPrecedes->pNext;
            pPrecedes->pNext = pNew;
        }
        
        // if pPrecedes is NULL set the new node as the head of the linked list
        else
        {
            pNew->pNext = *ppHead;
            *ppHead = pNew;
        }
    }
     
    // return the new Node (NULL or location)
    return pNew;
}

/***********************************************************************************************
    Node *searchLL(Node *pHead, char szMatchTeamId[ ], Node **ppPrecedes)
Purpose:
    Searches the linked list for a matching node corresponding to the team id. If the id already
    exists, it returns the pointer to the node that contains it. If the team does not exist, the 
    functions returns NULL and exits.
Parameters:
    *pHead                       Pointer to the start of the linked list
    szMatchTeamId[ ]             String of the team id being checked
    **ppPrecedes                 Pointer to the pointer of the preceding node 
Notes:
***********************************************************************************************/
Node *searchLL(Node *pHead, char szMatchTeamId[ ], Node **ppPrecedes)
{
    Node *p;
    Node *q;

    // Check to ensure node is NULL or if team is first in list
    if (pHead == NULL || strcmp(szMatchTeamId, pHead->team.szTeamId) == 0)
    {
        *ppPrecedes = NULL;
        return pHead;
    }
        
    // if the team id does not match the head id return NULL
    if (strcmp(szMatchTeamId, pHead->team.szTeamId) < 0)
    {
        *ppPrecedes = NULL;
        return NULL;
    }
        
    q = pHead; 

    // iterate through the linked list to check for matching id's for
    // location in the linked list
    for (p = pHead->pNext; p != NULL; p = p->pNext)
    {
        // if the teams match update the previous node location
        // and return the current node location
        if (strcmp(szMatchTeamId, p->team.szTeamId) == 0)
        {
            *ppPrecedes = q; 
            return p;
        }
        
        // if the teams do not match update the previous location and return NULL
        if (strcmp(szMatchTeamId, p->team.szTeamId) < 0)
        {
            *ppPrecedes = q;
            return NULL;
        }

        q = q->pNext;
    }

    *ppPrecedes = q;
    return NULL;
}

