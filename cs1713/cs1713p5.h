/**********************************************************************
cs1713p5.h
Purpose:
    Defines typedefs for
        Team - includes team ID, team name, number of wins, number of losses, 
               email address, phone, contact full name, fee amount, and 
               amount paid.
        Game - game information including team 1, team 2, score 1, and score 2
        NodeT - tree NodeT for one team
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

// NodeT structure for the tree
typedef struct NodeT
{
    Team team;                          // contains team info for one team
    struct NodeT *pLeft;                // left NodeTin a tree
    struct NodeT *pRight;               // right NodeTin a tree
} NodeT;


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
#define ERR_COMMAND_FILENAME    "invalid Command file name "
#define ERR_INVALID_COMMAND     "command name is invalid"
#define ERR_TEAM_ID_DATA        "invalid Team id data "
#define ERR_TEAM_CONTACT_DATA   "invalid Team contact data "
#define ERR_GAME_DATA           "invalid game data"

#define ERR_TEAM_SHOW_DATA     "bad data for TEAM SHOW command "
#define ERR_TEAM_PAID_DATA     "bad data for TEAM PAID command "
#define ERR_TEAM_NEW_DATA     "bad data for TEAM PAID command "
#define ERR_GAME_RESULT_DATA   "bad data for TEAM RESULT command "
#define ERR_GAME_FIX_DATA      "bad data for GAME FIX command "
#define ERR_TEAM_SUB_COMMAND   "invalid/missing subcommand for TEAM"
#define ERR_GAME_SUB_COMMAND   "invalid/missing subcommand for GAME"

#define WARN_TEAM_NOT_FOUND     " *** team (%s) not found "
#define WARN_GAME_WAS_TIE       " *** game was a tie "
#define WARN_SAME_TEAM          " *** same team "
#define WARN_TEAM_SUB_COMMAND   "invalid subcommand for TEAM: %s\n"
#define WARN_GAME_SUB_COMMAND   "invalid subcommand for GAME: %s\n"
#define WARN_TEAM_NEW_SUB_COMMAND "invalid TEAM NEW subcommand: %s\n"
#define WARN_TEAM_CONTACT_SUB_COMMAND "invalid TEAM CONTACT subcommand: %s\n"
#define WARN_TEAM_ALREADY_EXISTS " *** team (%s) already exists"



/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

/* prototypes */

/* functions Larry provided */
NodeT *allocateNodeT(Team Team);
NodeT *getTeams(char * pszTeamFileName);
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);

/* functions you must code  */
// functions from the notes, but you must change slightly
NodeT *searchT(NodeT *p, char szMatchTeamId[]);
NodeT *insertT(NodeT *p, Team team);
void prettyPrintT(NodeT *p, int iIndent);
void printInOrder(NodeT *p);

// functions from program 4 that you must change for the tree
void printTeams(char *pszHeading, NodeT *pRoot);
void processGame(NodeT *pRoot, Game game);
void processGameFix(NodeT *pRoot, Game game, int iOldScore1, int iOldScore2);
void processGameCommand(NodeT *pRoot
    , char *pszSubCommand, char *pszRemainingInput);
void processTeamCommand(NodeT **ppRoot
    , char *pszSubCommand, char *pszRemainingInput);
