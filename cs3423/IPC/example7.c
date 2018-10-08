#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main (int argc, char *argv[])
{
    long lForkPid;
    long lMyVal = 10;      // a variable to demonstrate
                           // that each process has their
                           // own copy
    // create a child process
    lForkPid = fork();  
    // Both the parent and child continue here
    switch(lForkPid)
    {
        case -1:
            errExit("fork failed: %s", strerror(errno));
            break;
        case 0:  // child process
            printf("Child Process: PID=%ld, PPID=%ld\n"
                 , (long) getpid(), (long) getppid());
            if (argc > 1)
            {   // child sleeps while parent termiantes
                sleep(8);
                printf("Child Done after sleep\n");
            }
            break;
        default: // parent process
            printf("Parent Process: PID=%ld, PPID=%ld\n"
                 , (long) getpid(), (long) getppid());
            printf("Parent Process: my child's PID=%ld\n"
                 , lForkPid);
            if (argc <= 1)
            {  // parent sleeps while child terminates
                sleep(8);
                printf("Parent Done after sleep\n");
            }
    }
    lMyVal += 5;
    printf("My PID=%ld, lMyVal=%ld\n"
         , (long) getpid(), lMyVal);
    return 0;
}
