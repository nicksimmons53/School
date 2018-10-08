#include <stdio.h>
#include <unistd.h>
int main ()
{
    long lPid;
    long lPPid;
    lPid = getpid();
    lPPid = getppid();
    printf("PID=%ld, PPID=%ld\n", lPid, lPPid);
    return 0;
}
