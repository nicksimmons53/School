#include <stdio.h>
#include <unistd.h>
int main ()
{
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("Eff UID=%d, Eff GID=%d\n", getuid(), getgid());
    return 0;
}
