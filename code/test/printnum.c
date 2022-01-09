/* printnum.c
 *	Test program print an integer to console.
 *	
 *	Just do PrintNum syscall.
 *
 */

#include "syscall.h"

int main()
{
	PrintNum(3001);
	Halt();
}
