#include "syscall.h"
char arr[10] = "Pong\n";
int main()
{
    int i = 0;
    for (i = 0; i < 5; ++i)
    {
        PrintString(arr);
    }
}