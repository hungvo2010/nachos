#include "syscall.h"
char F[10] = "Pong\n";
int main()
{
    int i = 0;
    int pid = GetPID();
    PrintNum(pid);
    for (i = 0; i < 5; ++i)
    {
        PrintString(F, 10);
    }
}