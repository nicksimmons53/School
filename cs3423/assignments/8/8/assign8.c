#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "cmdparse.h"

#define BUFFER_SIZE 100

int getShellCommand( );
void processCommand(CMD *cmd);
void pipelining(CMD *cmd);
void background(CMD *cmd);
void singleCommand(CMD *cmd); 
void redirection(CMD *cmd, int type);

int main( ) {
    getShellCommand( );
    return 0;
}

int getShellCommand( ) {
    char buffer[100];
    char command[BUFFER_SIZE - 1];

    printf("$ ");

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        CMD cmd;
        int parse;

        sscanf(buffer, "%[^\t]", command);

        if (strcmp(command, "exit") == 0)
            return 0;

        parse = cmdparse(command, &cmd);
        if (parse == PARSE_ERROR)
            continue;
        
        processCommand(&cmd);

        printf("$ ");
    }

    return 0;
}

void processCommand(CMD *cmd) {
    int flag = 0;
    int type;
    
    if (cmd->pipelining == TRUE) {
        flag = 1;
        pipelining(cmd);
    } 

    if (cmd->background == TRUE) {
        flag = 1;
        background(cmd);
    }

    if (cmd->redirectIn == TRUE) {
        flag = 1;
        redirection(cmd, 1);
    }

    if (cmd->redirectOut == TRUE) {
        flag = 1;
        //redirection(cmd, 2);
    }

    if (cmd->redirectAppend == TRUE) {
        flag = 1;
        //redirection(cmd, 3);
    }
    
    if (flag == 0) {
        singleCommand(cmd);
    }
}

void redirection(CMD *cmd, int type) {
    long forkStatus;
    long forkStatus2;
    int in, out, append;
    int flag;
    int fdM[2];

    if (cmd->infile == NULL && cmd->outfile == NULL) {
        fprintf(stderr, "unable to open file for redirection\n");
        return;
    }
    
    in = open(cmd->infile, O_RDONLY);
    out = open(cmd->outfile, O_WRONLY | O_CREAT, 0644); 
    append = open(cmd->outfile, O_APPEND | O_WRONLY, 0644);

    forkStatus = fork( );
    switch (forkStatus) {
        case -1:
            fprintf(stderr, "Fork failed\n");
            break;
        case 0:
            // STDIN
            if (in != -1) {
                if (dup2(in, 0) == -1)
                    fprintf(stderr, "Failed redirect stdin\n");
                close(in);
                execvp(cmd->argv1[0], cmd->argv1);
            } 

            // STDOUT
            if (out != -1) {
                if (dup2(out, 1) == -1)
                    fprintf(stderr, "Failed redirect stdout\n");
                close(out);
                execvp(cmd->argv1[0], cmd->argv1);
            }

            // APPEND
            if (append != -1) {
                if (dup2(append, 1) == -1)
                    fprintf(stderr, "Failed redirect append\n");
                close(append);
                execvp(cmd->argv1[0], cmd->argv1);
            }
            
            close(in); close(out); close(append);
            break;
    }
}

void pipelining(CMD *cmd) {
    long forkStatus;
    long forkStatus2;
    int fdM[2];
    int read; int write;
    int exitStatus = 0;
    int exitStatus2 = 0;
    
    if (pipe(fdM)== -1) 
        fprintf(stderr, "Pipe not Created\n");
    forkStatus = fork( );
    switch (forkStatus) {
        case -1:
            fprintf(stderr, "Fork failed\n");
            break;
        case 0:
            if (dup2(fdM[1], STDOUT_FILENO) == -1)
                fprintf(stderr, "Failed to redirect stdout\n");
            close(fdM[0]); close(fdM[1]);
            execvp(cmd->argv1[0], cmd->argv1);
        default:
            forkStatus2 = fork( );
            switch (forkStatus2) {
                case -1:
                    fprintf(stderr, "Failed to fork second child\n");
                    break;
                case 0:
                    if (dup2(fdM[0], STDIN_FILENO) == -1) 
                        fprintf(stderr, "Failed to redirect stdin\n");
                    close(fdM[0]); close(fdM[1]);
                    execvp(cmd->argv2[0], cmd->argv2);
                    break;
                default:
                    close(fdM[0]); close(fdM[1]);
                    int i;
                    for (i = 0; i < 2; i++) {
                        int iExitStatus = 0;
                        wait(&iExitStatus);
                    }
                    fflush(stderr);
            }
    }
}

void background(CMD *cmd) {
    int exitStatus = 0;
    long forkStatus;

    forkStatus = fork( );
    switch (forkStatus) {
        case -1:
            fprintf(stderr, "Child process failed to exec\n");
            fflush(stderr);
            break;
        case 0:
            execvp(cmd->argv1[0], cmd->argv1);
            break;
    }
}

void singleCommand(CMD *cmd) {
    int exitStatus = 0;
    long forkStatus;
    long waitPID;

    forkStatus = fork( );
    switch (forkStatus) {
        case -1:
            fprintf(stderr, "Child process failed to exec\n"); 
            fflush(stderr);
            break;
        case 0:
            execvp(cmd->argv1[0], cmd->argv1);
            break;
        default:
            waitPID = wait(&exitStatus);
            if (waitPID == -1)
                fprintf(stderr, "Wait Error.");
            fflush(stderr);
    }
}
