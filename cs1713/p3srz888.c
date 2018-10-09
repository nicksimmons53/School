/**********************************************************************
p3srz888.c by your Nicholas Simmons
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
#include "cs1713p3.h"

/******************** getTeams **************************************
    int getTeams(Team teamM[], char * pszTeamFileName)
Purpose:
    Retrieves the teams from the specified file.
Parameters:
    O   Team teamM[]          an array which is used for returning the teams.
    I   char *pszTeamFileName name of the file containing the team data
Returns:
    A count of the number teams.
Notes:
    1. This opens the file based on the specified file name.
    2. The data has two types of records per team:    
    - Team Identification Information:
        o One line per team (separated by spaces)
            szTeamId  iWins  iLosses  dFeeAmount  dPaidAmount  
             6s       d      d        lf          lf
    - Team Contact Information:
        o One line per team (separated by commas)
            szTeamNm  szEmail szPhone szContactName
            12s       30s     13s     20s
        o Although szTeamNm is a maximum of 12 characters, it may 
          contain spaces; therefore, you cannot simply use %13s.  
          For szFullName, you will have to use a bracket format 
          code using %[^,].  The next two values are also terminated by 
          commas.
        o For szContactName, it contains spaces and is terminated by
          a new line character.  You will have to use a bracket format code 
          using %[^\n]
**************************************************************************/

int getTeams(Team teamM[], char * pszTeamFileName)
{
    char szInputBuffer[100];    // input buffer for reading data
    int i = 0;                  // subscript in teamM
    int iScanfCnt;              // returned by sscanf
    FILE *pFileTeam;            // Stream Input for Teams data. 
    Team team;
    int scanCount, scanCount2;

    /* open the Teams stream data file */
    if (pszTeamFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-t");

    pFileTeam = fopen(pszTeamFileName, "r");
    if (pFileTeam == NULL)
        exitError(ERR_TEAM_FILENAME, pszTeamFileName);

    /* get team information until EOF
    ** fgets returns null when EOF is reached.
    */

    /**** your code ******/
    while (!feof(pFileTeam))
    {
        fgets(szInputBuffer, MAX_LINE_SIZE, pFileTeam);
        scanCount = sscanf(szInputBuffer, "%6s %2d %2d %lf %lf\n"
            , teamM[i].szTeamId
            , &teamM[i].iWins
            , &teamM[i].iLosses
            , &teamM[i].dFeeAmount
            , &teamM[i].dPaidAmount);

        if (scanCount != 5)
        {
            printf("Invalid Team Data\n");
            exit(-1);
        }

        fgets(szInputBuffer, MAX_LINE_SIZE, pFileTeam);
        scanCount2 = sscanf(szInputBuffer, "%12[^,],%30[^,],%13[^,],%20[^\n]\n"
            , teamM[i].szTeamName
            , teamM[i].szEmailAddr
            , teamM[i].szPhone
            , teamM[i].szContactname);

        if (scanCount2 != 4)
        {
            printf("Invalid Team Data\n");
            exit(-1);
        }

        i++;
    }
 
    fclose(pFileTeam);
    return i;
}

/******************** sortTeams **************************************
    void sortTeams(Team teamM[], int iTeamCnt)
Purpose: Bubble sort by the teams' ID's into alphabetical order to make binary search
         easier and more effective.

Parameters: char Team Struct Array
            int  iTeamCnt      
  
Returns: Void 
    
Notes:
    
**************************************************************************/
void sortTeams(Team teamM[], int iTeamCnt)
{
    int i, j;   // Counters for both for loops required
    Team temp;  // temp array for storage of original data


    for (i = 0; i < (iTeamCnt - 1); i++)
    {
        for (j = 0; j < (iTeamCnt - i - 1); j++)
        {
            // Comparing one ID to the next for alphabetical sort
            if (strcmp(teamM[j].szTeamId, teamM[j + 1].szTeamId) > 0)
            {
                // Swapping of data
                temp = teamM[j];
                teamM[j] = teamM[j + 1];
                teamM[j + 1] = temp;
            }
        }
    }
}

/******************** printTeams *****************************
   void printTeams(char szHeading[], Team teamM[], int iTeamCnt)
Purpose:
    Prints team information. 
Parameters:
    i char szHeading[]  string describing why the teams are printed
                        (e.g., "Original Team Information")
    i Team teamM[]      array of teams
    i int iTeamCnt      number of teams
Notes:
    Prints two lines of text per team.
**************************************************************************/
void printTeams(char szHeading[], Team teamM[], int iTeamCnt)
{
    int i;          // local counter
    // Print the heading
    printf("%s\n", szHeading);
    printf("  %-6s %-12s %4s %4s  %-8s  %-8s\n"
        , "Id", "Team Name", "Wins", "Loss", "Fee Amt", "Paid Amt");
    printf("\t\t  %-20s %-13s %s\n"
        , "Contact Name", "Phone", "Email");
    // iterate through the teamM array
    for (i = 0; i < iTeamCnt; i++)
    {
        printf("  %-6s %-12s %4d %4d  %8.2lf  %8.2lf\n"
            , teamM[i].szTeamId
            , teamM[i].szTeamName
            , teamM[i].iWins
            , teamM[i].iLosses
            , teamM[i].dFeeAmount
            , teamM[i].dPaidAmount);
        printf("\t\t  %-20s %-13s %s\n"
            , teamM[i].szContactname
            , teamM[i].szPhone
            , teamM[i].szEmailAddr);
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
    I/O Team teamM[]                  Array of teams
    I   int   iTeamCnt                Number of elements in teamM[]
    I   char  *pszSubCommand          Should be PAID or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand). 
Notes:

**************************************************************************/
void processTeamCommand(Team teamM[], int iTeamCnt
     , char *pszSubCommand, char *pszRemainingInput)
{
    int iScanfCnt;
    char szTeamId[7];           // temporary Team Id
    int i;                      // subscript of a team in the team array
    double dAmount;
    
    // Determine what to do based on the PAID subCommand
    if (strcmp(pszSubCommand, "PAID") == 0)
    {
        // get the szTeamId and dAmount
        iScanfCnt = sscanf(pszRemainingInput, "%6s %lf", szTeamId, &dAmount);

        // Check for bad data
        if (iScanfCnt < 2)
            exitError(ERR_TEAM_PAID_DATA, pszRemainingInput);
        
        // subscript of szTeamId location in teamM[]
        i = searchTeams(teamM, iTeamCnt, szTeamId);

        // if szTeamId is not found exit function
        if (i == -1)
        {
            printf(" *** team (%s) not found\n", szTeamId);
            return;
        }

        // add the paid amount to the team's previous paid amount
        teamM[i].dPaidAmount += dAmount;

   }
   
   // Determine what to do based on the SHOW subCommand
    else if (strcmp(pszSubCommand, "SHOW") == 0)
    {
        // get the szTeamId
        iScanfCnt = sscanf(pszRemainingInput, "%6s", szTeamId);
        
        // Check for bad data
        if (iScanfCnt < 1)
            exitError(ERR_TEAM_SHOW_DATA, pszRemainingInput);
        
        // find the team 
        i = searchTeams(teamM, iTeamCnt, szTeamId);
        if (i < 0)
        {
            printf(WARN_TEAM_NOT_FOUND, szTeamId);
            printf("\n");
            return;
        }

        // print the team data for SHOW command
        else if (i > 0)
        {
        printf("  %-6s %-12s %4d %4d  %8.2lf  %8.2lf\n"
            , teamM[i].szTeamId
            , teamM[i].szTeamName
            , teamM[i].iWins
            , teamM[i].iLosses
            , teamM[i].dFeeAmount
            , teamM[i].dPaidAmount);
        printf("\t\t  %-20s %-13s %s\n"
            , teamM[i].szContactname
            , teamM[i].szPhone
            , teamM[i].szEmailAddr);
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
void processGameCommand(Team teamM[], int iTeamCnt
                             , char *pszSubCommand, char *pszRemainingInput)
{
    Game game;
    int iScanfCnt;
    int i, j;             // subscripts of team locations in the array
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
        if(strcmp(game.szTeamId1, game.szTeamId2) == 0)
        {
            printf("%s\n", WARN_SAME_TEAM);
            return;
        }
        
        // subscript location of both teams in teamM[]
        i = searchTeams(teamM, iTeamCnt, game.szTeamId1);
        j = searchTeams(teamM, iTeamCnt, game.szTeamId2);
        
        // if either team are not found print a warning and exit function
        if (i == -1)
        {
            printf(" *** team(%s) not found\n", game.szTeamId1);
            return;
        }

        if (j == -1)
        {
            printf(" *** team(%s) not found\n", game.szTeamId2);
            return;
        }

        processGame(game, teamM, iTeamCnt);   // call processGame to run data that is being scanned
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
        i = searchTeams(teamM, iTeamCnt, game.szTeamId1);
        j = searchTeams(teamM, iTeamCnt, game.szTeamId2);

        // If either team is not found the funtion is exited
        // and the wins and losses are not affected
        if (i == -1)
        {
            printf(" *** team (%s) not found\n", game.szTeamId1);
            return;
        }

        if (j == -1)
        {
            printf(" *** team (%s) not found\n", game.szTeamId2);
            return;
        }
        
        // Call to processGameFix to process the wins and losses 
        // according to the new scores
        processGameFix(game, teamM, iTeamCnt, iOldScoreTeam1, iOldScoreTeam2);
    }
}
/******************** searchTeams *****************************
    int searchTeams(Team teamM[], int iTeamCnt, char *pszMatchTeamId)
Purpose:
    Using a binary search, it finds the specified team in the teamM array.
Parameters:
    I   Team teamM[]                 Array of teams
    I   int   iTeamCnt               Number of elements in teamM[]
    I   char *pszMatchTeamId         Team Id to find in the array
Returns:
    >= 0   subscript of where the match value was found
    -1     not found
Notes:

**************************************************************************/
int searchTeams(Team teamM[], int iTeamCnt, char *pszMatchTeamId)
{
    int lower = 0;      // Counter for lower bound index
    int upper = iTeamCnt - 1;       // Counter for upper bound index
    int middle;     // Counter for middle position index
    
    // Binary search the sorted teams for subscript locations
    while (lower <= upper)
    {
        middle = (lower + upper)/2;

        if (strcmp(pszMatchTeamId, teamM[middle].szTeamId) == 0)
            return middle;
        if (strcmp(pszMatchTeamId, teamM[middle].szTeamId) < 0)
            upper = middle - 1; 
        else
            lower = middle + 1; 
    }
    return -1;		// not found
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
void processGame(Game game, Team teamM[ ], int iTeamCnt)
{
    int i, index1, index2;

    // Binary search for team index
    index1 = searchTeams(teamM, iTeamCnt, game.szTeamId1);
    index2 = searchTeams(teamM, iTeamCnt, game.szTeamId2);
    
    // Analyze the scores of both games
    if (game.iScore1 > game.iScore2)
    { 
        teamM[index1].iWins++;
        teamM[index2].iLosses++;
    }

    if (game.iScore1 < game.iScore2)
    {
        teamM[index1].iLosses++;
        teamM[index2].iWins++;
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
void processGameFix(Game game, Team teamM[], int iTeamCnt, int iOldScore1, int iOldScore2)
{
    int index1, index2;
    int iNewScore1, iNewScore2;
    
    // Locations of both team IDs in the array
    index1 = searchTeams(teamM, iTeamCnt, game.szTeamId1);
    index2 = searchTeams(teamM, iTeamCnt, game.szTeamId2);
    
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
            teamM[index1].iWins--;
            teamM[index2].iLosses--;
        }

        if (iOldScore1 < iOldScore2)
        {
            teamM[index1].iLosses--;
            teamM[index2].iWins--;
        }
    }
    
    // If the new scores are not equal to the old scores process the new scores
    if (iOldScore1 != game.iScore1 || iOldScore1 != game.iScore2)
    {
        // Correct the win and loss columns if team1 wins
        if (game.iScore1 > game.iScore2)
        {
            teamM[index1].iWins++;
            teamM[index1].iLosses--;
            teamM[index2].iWins--;
            teamM[index2].iLosses++;
        }
        
        // Correct the win and loss columns if team2 wins
        if (game.iScore1 < game.iScore2)
        {
            teamM[index1].iWins--;
            teamM[index1].iLosses++;
            teamM[index2].iWins++;
            teamM[index2].iLosses--;
        }
    }
}
