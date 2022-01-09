#include "syscall.h"

char childprocess1[5] = "ping";
char childprocess2[5] = "pong";
char tmp[10] = "abcabc\n";
int main()
{
	int x, y;

    x = Exec(childprocess1);
    y = Exec(childprocess2);
    Join(x);
    Join(y);
    //PrintNum(y);

}
