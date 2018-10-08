/******************** msleep **************************************
  int msleep(unsigned int iMsec)
Purpose:
    Sleeps for at least the specified number of microseconds
Parameters:
    I   unsigned int iMsec  Number of microseconds to sleep
Returns:
    0 - normal return
    -1 - system error, examine errno
Notes:
    - Uses nanosleep.
    - Continues with sleeping after an interrupt
**************************************************************************/
#include <time.h>
#include <signal.h>
#include <errno.h>
int msleep(unsigned int msec)
{
    struct timespec timeReq;
    struct timespec timeRemain;
    // Set the seconds and nanoseconds from the microseconds
    timeRemain.tv_sec = msec / 1000;
    timeRemain.tv_nsec = (msec % 1000) * (1000000L);
    timeReq = timeRemain;
    // Handle interrupts
    while(nanosleep(&timeReq, &timeRemain) == -1) 
    {
        if (errno != EINTR)
            return -1;
        timeReq = timeRemain;
    }
    return 0;
}