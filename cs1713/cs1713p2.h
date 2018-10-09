/**********************************************************************
cs1713p2.h
Purpose:
    Defines typedefs for
        Team - includes team ID, team name, number of wins, number of losses, 
               email address, phone, contact full name, fee amount, and 
               amount paid.
        Game - game information including team 1, team 2, score 1, and score 2.
    Defines constants for
        boolean values
        error messages
        program return codes
    Prototypes
Notes:
    
**********************************************************************/

/**** typedefs ****/

// Team information 
typedef struct
{
    char        szTeamId[7];            // 6 character Team ID
    char        szEmailAddr[31];        // Team's email address 
    char        szTeamName[13];         // Team full name
    char        szPhone[14];            // Team contact phone number (999)999-9999
    char        szContactname[21];      // Team contact first and last name
    int         iWins;                  // Number of wins
    int         iLosses;                // Number of losses
    double      dFeeAmount;             // Amount of fees
    double      dPaidAmount;            // Amount this team has paid
} Team;

// Game Information
typedef struct
{
    char        szTeamId1[7];           // first team for the game
    char        szTeamId2[7];           // second team for the game
    int         iScore1;                // score of the first team
    int         iScore2;                // score of the second team
} Game;


/**** constants ****/
// Maximum sizes 
#define MAX_LINE_SIZE 100
#define MAX_TEAMS 20                    // maximum number of teams in the array

// boolean
#define FALSE           0
#define TRUE            1

/*  Error Messages */
#define ERR_MISSING_SWITCH      "missing switch "
#define ERR_EXPECTED_SWITCH     "expected switch, found "
#define ERR_MISSING_ARGUMENT    "missing argument for "
#define ERR_TEAM_FILENAME       "invalid Team file name "
#define ERR_TEAM_ID_DATA        "invalid Team id data "
#define ERR_TEAM_CONTACT_DATA   "invalid Team contact data "
#define ERR_GAME_DATA           "invalid game data"
#define WARN_TEAM_NOT_FOUND     " *** team (%s) not found "
#define WARN_GAME_WAS_TIE       " *** game was a tie "
#define WARN_SAME_TEAM          " *** same team "

/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

/* prototypes */

/* functions you must code  */
int getTeams(Team teamM[]);
void processGameFile(Team teamM[], int iTeamCnt);
void processGame(Game game, Team teamM[], int iTeamCnt);
int findTeam(Team teamM[], int iTeamCnt, char szTeamId[]);

/* other functions */
void printTeams(char szHeading[], Team teamM[], int iTeamCnt);

void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);