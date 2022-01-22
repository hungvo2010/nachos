#include "syscall.h"
char arr[20] = "Pinggg\n";
int main()
{
    int i = 0;
    int pid = GetPID();
    PrintNum(pid);
    for (i = 0; i < 10; ++i)
    {
        PrintString(arr, 10);
    }
}