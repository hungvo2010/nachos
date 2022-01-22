#include "syscall.h"

char output[20] = "output.txt";
char voinuoc[10] = "voinuoc";
char tmp[10];
char buffer[3] = "  ";
int main()
{
    int fileid1;
    int pid;
    int count = 0;
    int i = 0;
    int rand_num;
    pid = GetPID();
    

    while (count < 10)
    {
        Wait(voinuoc);
        fileid1 = Open(output, 2);

        buffer[0] = (char)('0' + pid);
        rand_num = RandomNum() % 10000;
        for (i = 0; i < rand_num; ++i);
	    Write(buffer, 2, fileid1);
        count++;

        Close(fileid1);
        Signal(voinuoc);
    } 
    return 0;
}
