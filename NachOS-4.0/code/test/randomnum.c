/* randomnum.c
 *	Test rogram generate a random integer, then print it to console.
 *	
 *	Do RandomNum and PrintNum syscall.
 *
 */

#include "syscall.h"
int
main()
{
	int s;
	s = RandomNum();
	PrintNum(s);
	Halt();
}
