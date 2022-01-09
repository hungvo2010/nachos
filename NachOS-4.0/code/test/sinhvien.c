#include "syscall.h"

char outputFile[20] = "output.txt";
char semaphore_name[10] = "voinuoc";
char tmp[10];
char buffer[3] = "  ";
int main()
{
    int fd1;
    int pid;
    int current_amount = 0;
    int i = 0;
    int rand_num;
    pid = GetPID();
    

    while (current_amount < 10)
    {
        Wait(semaphore_name);
        fd1 = Open(outputFile, 2); // append

        buffer[0] = (char)('0' + pid);
        rand_num = RandomNum() % 10000;
        for (i = 0; i < rand_num; ++i);
	    Write(buffer, 2, fd1);
        current_amount++;

        Close(fd1);
        Signal(semaphore_name);
    } 
    return 0;
}
