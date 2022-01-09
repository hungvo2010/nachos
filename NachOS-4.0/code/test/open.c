#include "syscall.h"
char F[10] = "a.txt";
char G[20];
char U[20] = "text";
int main()
{
    int fd1, fd2;
    int num_read;
    
	fd1 = Open(F,2);
    //Read(G, 3, fd1);
    num_read = Write(U, 7, fd1);
    num_read = Write(U, 7, fd1);

    Close(fd1);
}