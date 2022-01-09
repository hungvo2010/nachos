#include "syscall.h"

char childprocess[5] = "help";
char tmp[10] = "abcabc\n";
int main()
{
	int x, y;

    x = CreateSemaphore(tmp, 1);
    Wait(tmp);

    Signal(tmp);
    PrintNum(x);

}
