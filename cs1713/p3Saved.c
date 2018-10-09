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
            printf("Invalid Team Data");
            exit(-1);
        }

        fgets(szInputBuffer, MAX_LINE_SIZE, pFileTeam);
        scanCount2 = sscanf(szInputBuffer, "%12[^,],%30[^,],%13[^,],%20[\n]\n"
            , teamM[i].szTeamName
            , teamM[i].szEmailAddr
            , teamM[i].szPhone
            , teamM[i].szContactname);

        if (scanCount2 != 4)
        {
            printf("Invalid Team Data");
            exit(-1);
        }

        i++;
    }
 
    fclose(pFileTeam);
    return i;
}

/******************** sortTeams **************************************
    void sortTeams(Team teamM[], int iTeamCnt)
Purpose:

Parameters:
  
Returns:
    
Notes:
    
**************************************************************************/
void sortTeams(Team teamM[], int iTeamCnt)
{
    /**** your code ******/


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
    
    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "PAID") == 0)
    {
        // get the szTeamId and dAmount
        // your code

        // Check for bad data
        if (iScanfCnt < 2)
            exitError(ERR_TEAM_PAID_DATA, pszRemainingInput);
        
        // your code which uses searchTeams and updates the paid amount

    }
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
        // your code

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

    // Determine what to do based on the subCommand
    // your code

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
    // your code

    return -1;		// not found
}


/*** include your processGame code from program #2.  Change it to use
     your binary search
***/
// your code


// comment
void processGameFix(Game game, Team teamM[], int iTeamCnt, int iOldScore1, int iOldScore2)
{
 // your code
}
