#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main () {
    long lForkPidLs, lForkPidSort;
    int iExitStatus = 0;    // Used by parent to get status of child
    long lWaitPid;          // PID of child that terminated
    int iReadFd, iWriteFd;  // File Descriptors for reader and writer
    int fdM[2];             // contains file descriptors for the pipe
    // create the pipe
    if (pipe(fdM) == -1)
        errExit("pipe not created: %s", strerror(errno));
    lForkPidLs = fork();    // create a child process 
    // Both the parent and first child continue here
    switch(lForkPidLs) {
        case -1:
            errExit("fork failed: %s", strerror(errno));
        case 0:  // child process - ls -l /usr/bin/
            if ( dup2(fdM[1], STDOUT_FILENO) == -1)
                errExit("Failed to rediect stdout for 'ls': %s"
                    , strerror(errno));
            close(fdM[0]); close(fdM[1]);
            execl("/bin/ls", "ls", "-l", "/usr/bin", NULL);
            errExit("Failed to exec 'ls -l /usr/bin': %s"
                , strerror(errno));
        default: // parent process
            // create the other child
            lForkPidSort = fork();    // create a child process 
            // Both the parent and second child continue here
            switch(lForkPidSort) {
                case -1:
                    errExit("fork of second child failed: %s"
                        , strerror(errno));
                case 0:  // child process - sort -k5 -n
                    if ( dup2(fdM[0], STDIN_FILENO) == -1)
                        errExit("Failed to rediect stdin for 'sort': %s"
                            , strerror(errno));
                    close(fdM[0]); close(fdM[1]);
                    execl("/usr/bin/sort", "sort", "-k5", "-n", NULL);
                    errExit("Failed to exec 'sort -k5 -n': %s"
                        , strerror(errno));
                default:  // parent process
                    close(fdM[0]); close(fdM[1]);
                    fprintf(stderr, "PID for ls: %ld, PID for sort: %ld\n"
                        , lForkPidLs, lForkPidSort);
                    fflush(stderr);
                    int i;
                    for (i = 0; i < 2; i+=1) {
                        iExitStatus = 0;
                        lWaitPid = wait(&iExitStatus); 
                        if (lWaitPid == -1)
                            errExit("wait failed: %s\n"
                                , strerror(errno));
                        if (lWaitPid == lForkPidLs)
                            fprintf(stderr,"First child terminated\n");
                        else
                            fprintf(stderr,"Second child terminated\n");
                        fflush(stderr);
                    }
                    fprintf(stderr,"complete\n");
            }
    }
    return 0;
}
