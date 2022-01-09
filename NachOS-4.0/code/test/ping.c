#include "syscall.h"
char F[10] = "Ping\n";
int main()
{
    int i = 0;
    for (i = 0; i < 5; ++i)
    {
        PrintString(F);
    }
}