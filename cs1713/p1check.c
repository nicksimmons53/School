/**********************************************************************
p1srz888.c  by Simmons, Nicholas 
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

    char szInputBuffer[MAX_LINE_SIZE + 1];

    Team team; 
    pFileTeam = stdin;

    Team teamM[MAX_TEAMS];
    int iTeamCount = getTeams(szInputBuffer, team, teamM);
    pFileGame = stdin;


    // Process the command switches
    processCommandSwitches(argc, argv,  &pszTeamFileName, &pszGameFileName);
    
    // open the Team stream data file
    if (pszTeamFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-t");
    
    pFileTeam = fopen(pszTeamFileName, "r");
    if (pFileTeam == NULL)
        exitError(ERR_TEAM_FILENAME, pszTeamFileName);

    // open the Game stream data file
    if (pszGameFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-t");

    pFileGame = fopen(pszGameFileName, "r");
    if (pFileGame == NULL)
        exitError(ERR_GAME_FILENAME, pszGameFileName);

    // print the team array
    printTeams("Original Team Info", teamM, iTeamCount);
   
    // get the Teams Data
    getTeams(szInputBuffer, team, teamM);

    // passing processGameFile
    processGameFile(teamM, iTeamCount);

    // print the resulting contents of the team array
    printTeams("Original Team Info", teamM, iTeamCount);


    fclose(pFileTeam);
    fclose(pFileGame);
    printf("\n");    // included so that you can put a breakpoint on this line
    return 0;
}

/****** you need to document and code this function *****
This function will read in data from a file and organize it into the proper
columns. The function will continue in a loop until the entire file is read.
*********************************************************/

void getTeams(char buffer[ ], Team team)
{
    int scanCount, scanCount2; 

    printf("%-7s %-13s %-7s %-7s %-7s %-7s \n %-22s %-14s %-31s \n"
         , "ID"
         , "Team Name"
         , "Wins"
         , "Loss"
         , "Fee Amt"
         , "Paid Amt"
         , "Contact Name"
         , "Phone"
         , "Email"); 

    while (!feof(pFileTeam)) 
    {
         if (buffer[0] == '\n')
             continue;

         fgets(buffer, MAX_LINE_SIZE, pFileTeam);
         scanCount = sscanf(buffer, "%6s %d %d %lf %lf" 
             , team.szTeamId
             , &team.iWins
             , &team.iLosses
             , &team.dFeeAmount
             , &team.dPaidAmount);

         if (scanCount < 5)
         {
             printf("%s", ERR_TEAM_ID_DATA);
             return;
         }
         
         fgets(buffer, MAX_LINE_SIZE, pFileTeam);
         scanCount2 = sscanf(buffer, "%12[^,],%30[^,],%13[^,],%20[^\n]"
             , team.szTeamName
             , team.szEmailAddr
             , team.szPhone
             , team.szContactname);
         if (scanCount < 4)
         {
             printf("%s", ERR_TEAM_ID_DATA);
             return;
         }

         printf("%-7s %-13s %7d %7d %7.2lf %7.2lf \n"
             , team.szTeamId
             , team.szTeamName
             , team.iWins
             , team.iLosses
             , team.dFeeAmount
             , team.dPaidAmount);

         printf("%-22s %-14s %-31s\n"
             , team.szContactname
             , team.szPhone
             , team.szEmailAddr);
    }
}

void printTeams(char szHeading[ ], Team teamM[ ], int iTeamCnt)
{
    int i;
    printf("%s\n", szHeading);
    printf("  %-6s %-12s %4s %4s  %-8s  %-8s\n"
        , "Id", "Team Name", "Wins", "Loss", "Fee Amt", "Paid Amt");
    printf("\t\t  %-20s %-13s %s\n"
        , "Contact Name", "Phone", "Email");
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
    , char **ppszGameFileName);
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
