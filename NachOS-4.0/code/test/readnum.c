/* readnum.c
 *	Test program read an integer from console, then print it back to console.
 *	
 *	Do ReadNum and PrintNum syscall.
 *
 */

#include "syscall.h"

int main()
{
	int x;
	x = ReadNum();
	PrintNum(x);
	Halt();
}
