#include "syscall.h"

int main()
{
    int fd1, fd2;
	fd1 = Open("a.txt", 1);
    fd2 = Open("a.txt", 0);
    PrintNum(fd1); PrintChar('\n');
    PrintNum(fd2); PrintChar('\n');

	Halt();
}