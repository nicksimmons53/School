/**********************************************************************
p2srz888.c  by Simmons, Nicholas 
(change the previous line for your abc123 ID and your name)
Purpose:
    This program reads team data to show current rankings.
Command Line Arguments:
    p1  -t  teamFileName  
Input:  
    Stream input file which contains many teams. There are two different 
    kinds of lines of data for each team:
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

Results:
    Print the list of teams as shown below.
    Examples:
        Id     Team Name    Wins Loss  Fee Amt   Paid Amt
                        Contact Name         Phone         Email
        UTSA01 Armadillos      8    0    150.00     80.00
                        Jean E Us            (210)555-1111 utsa@xyz.com
        COM001 Comm Eagles     7    1    150.00     75.00
                        Mae King             (210)555-2222 maeking@xyz.com
        SOUTH1 Slam Dunk       5    3    120.00     75.00
                        Jerry Tall           (210)555-3333 slamdunk@gmail.com
        ALHGHT Cake Eaters     4    4    175.00    100.00
                        E Z Street           (210)555-6666 sliverspoon@xyz.com
        UNKN01 Org New Blk     1    7    150.00     50.00
                        Bob Wire             (210)555-1234 bobwire@xyz.com
        NEWB01 River Rats      0    8    120.00     75.00
                        Rock D Boat          (210)555-4444 riverrat@xyz.com
        UNKN02 Hackers         3    5    150.00     75.00
                        Tom E Gunn           (210)555-5555 cyber@gmail.com


Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information
Notes:
    p1 -?  will provide the usage information.  In some shells,
                you will have to type reserve -\?
  
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p2.h"

FILE *pFileTeam;               // stream Input for Team data
FILE *pFileGame;

void processCommandSwitches(int argc, char *argv[]
    , char **ppszTeamFileName
    , char **ppszGameFileName);

int main(int argc, char *argv[])
{
    // Function Variables
    char *pszTeamFileName = NULL;
    char *pszGameFileName = NULL;

    Team team; 
    pFileTeam = stdin;

    Team teamM[MAX_TEAMS];
    Game game;
    pFileGame = stdin;
    int iTeamCount;

    // process command switches
    processCommandSwitches(argc, argv,  &pszTeamFileName, &pszGameFileName);
    
    // open the Team stream data file
    if (pszTeamFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-t");
    
    pFileTeam = fopen(pszTeamFileName, "r");
    if (pFileTeam == NULL)
        exitError(ERR_TEAM_FILENAME, pszTeamFileName);

    // Variable for amount of teams entered 
    iTeamCount = getTeams(teamM);

    // open the Game stream data file
    if (pszGameFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-t");

    pFileGame = fopen(pszGameFileName, "r");
    if (pFileGame == NULL)
        exitError(ERR_GAME_DATA, pszGameFileName);
   
    // get the Teams Data
    getTeams(teamM);

    // print the team array
    printTeams("Original Team Info", teamM, iTeamCount);

    // passing processGameFile
    processGameFile(teamM, iTeamCount);

    // print the resulting contents of the team array
    printTeams("Resulting Team Info", teamM, iTeamCount);


    fclose(pFileTeam);
    fclose(pFileGame);
    printf("\n");    // included so that you can put a breakpoint on this line
    return 0;
}

/****** you need to document and code this function *****
This function will read in data from a file and organize it into the proper
columns. The function will continue in a loop until the entire file is read.
*********************************************************/

int getTeams(Team teamM[ ])
{
    char szInputBuffer[MAX_LINE_SIZE + 1];
    int scanCount, scanCount2;
    int sum = 0;
    int i = 0;
    Team team;
    int j;
    
    // Read the entire game file until EOF is reached
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
        scanCount2 = sscanf(szInputBuffer, "%12[^,],%30[^,],%13[^,],%20[^\n]\n"
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

    return i;
}

/************************************************************************
    void printTeams(char szHeading[ ], Team teamM[ ], int iTeamCnt)
Purpose:
    Prints team information.
Parameters: 
    i char szHeading[ ]  string describing why the teams are printed 
                         (e.g., "Original Team Information")
    i Team teamM[ ]      array of teams
    i int iTeamCnt       number of teams
Notes:
    Prints two lines of text per team.
*************************************************************************/

void printTeams(char szHeading[ ], Team teamM[ ], int iTeamCnt)
{
    // local counter 
    int i;          

    // print the heading
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

/******************** processGameFile ************************************
Purpose: 
    This function will read from the game file until the EOF is reached.
Parameters:
    C   Array of team strings
    I   Count of teams entered
*************************************************************************/
void processGameFile(Team teamM[ ], int iTeamCnt)
{
       char szInputBuffer[MAX_LINE_SIZE + 1];   // buffer for fgets
       int scanCount;                           // int for amount of data entries
       Game game;                               // struct of the Game file

       printf("\n");
       printf("Processing Games\n");

       // opens the Game file until EOF is reached
       while(!feof(pFileGame))
       {
           if (szInputBuffer[0] == '\n')
               continue;
           
           // reads the game data one line at a time
           fgets(szInputBuffer, MAX_LINE_SIZE, pFileGame);
           scanCount = sscanf(szInputBuffer, "%7s %7s %d %d"
               , game.szTeamId1
               , game.szTeamId2
               , &game.iScore1      
               , &game.iScore2);

           // if there are not enough entries program quits reading the file
           if (scanCount != 4)
           {
               printf("%s", ERR_GAME_DATA);
               exit(-1);
           }
           
           // Call to processGame to check the game data and scores
           processGame(game, teamM, iTeamCnt);
       }
       printf("\n");
}

/******************** processGame ****************************************
Purpose:
    Processes the data from the game file and runs for each iteration.
Parameters:
    C   Struct of game file
    C   Array of team information
    I   Count of teams entered
*************************************************************************/
void processGame(Game game, Team teamM[ ], int iTeamCnt)
{
    int i;

    // Subscripts of both teams location
    int index1 = findTeam(teamM, iTeamCnt, game.szTeamId1);
    int index2 = findTeam(teamM, iTeamCnt, game.szTeamId2);
    
    // Print the game data 
    printf("%7s %7s %d %d"
        , game.szTeamId1
        , game.szTeamId2
        , game.iScore1
        , game.iScore2);
  
    // Analyze the scores of both games
    if (game.iScore1 > game.iScore2)
    {
        teamM[index1].iWins++;
        teamM[index2].iLosses++;
    }
    if (game.iScore1 < game.iScore2)
    {
        teamM[index2].iWins++;
        teamM[index1].iLosses++;
    }
    
    // Error warnings for Team ID's that are not found
    if ((index1) == -1)
        printf("\t(Team (%s) not found)", game.szTeamId1);
    if ((index2) == -1)
        printf("\t(Team (%s) not found)", game.szTeamId2);
    
    // Error warning if teams are the same
    if (strcmp(game.szTeamId1, game.szTeamId2) == 0)
        printf("\t%s", WARN_SAME_TEAM);
    
    // Error warning if the game was a tie
    if (game.iScore1 == game.iScore2)
        printf("\t%s", WARN_GAME_WAS_TIE);


    printf("\n");
}

/******************** findTeam *******************************************
Purpose: 
    Return the address of a specific Team ID in the array, return -1 if not 
    found.
Parameters:
    C   Array of team information
    I   Count of teams entered
    C   Array of team ID size
*************************************************************************/
int findTeam(Team teamM[ ], int iTeamCount, char szTeamId[ ])
{
    int i;
    
    // Loop cycles through the array of teams
    for (i = 0; i < iTeamCount; i++)
    {
        // strcmp will return 0 if teams are the same 
        if (strcmp(teamM[i].szTeamId, szTeamId) == 0)
            // i is the index of the team in the array
            return i;
    }

    return -1;      // return -1 if the team is not found
}

/******************** processCommandSwitches *****************************
void processCommandSwitches(int argc, char *argv[], char **ppszTeamFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                    Count of command line arguments
    I   char *argv[]                Array of command line arguments
    O   char **ppszTeamFileName     Team file name
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszTeamFileName
    , char **ppszGameFileName)
{
    int i;
    // Examine each command argument (except argument 0 which is the executable name)
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
            case 't':                   // Team File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else
                    *ppszTeamFileName = argv[i];
                break;

            case 'g':
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else 
                    *ppszGameFileName = argv[i];
                break;

            case '?':
                exitUsage(USAGE_ONLY, "", "");
                break;
            default:
                exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

/******************** exitError *****************************
    void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    Prints an error message and diagnostic to stderr.  Exits with
    ERROR_PROCESSING.
Parameters:
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
            , iArg
            , pszMessage
            , pszDiagnosticInfo);
    fprintf(stderr, "p1 -t TeamFileName\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
