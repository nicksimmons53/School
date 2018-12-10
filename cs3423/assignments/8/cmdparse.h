typedef struct cmd
{
    char *argv1[8];     // first command with args; ends with NULL
    char *argv2[8];     // second command with args; ends with NULL 
    int pipelining;     // TRUE if pipelining detected
    int background;     // TRUE if background detected 
    int redirectIn;     // TRUE if redirecting stdin
    int redirectOut;    // TRUE if redirecting stdout
    int redirectAppend; // TRUE if stdout redirection is append mode
    char *infile;       // corresponding file for stdin redirection
    char *outfile;      // corresponding file for stdout redirection
} CMD;

int isSpecialChar(char *str);
int cmdparse(char *cmdbuf, CMD *com);

#define PARSE_ERROR 1
#define NO_INPUT 2
#define TRUE 1
#define FALSE 0
