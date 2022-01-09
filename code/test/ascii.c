/* ascii.c
 *	Test rogram to print the ascii table.
 *	
 *	Just do PrintChar syscall.
 *
 */

#include "syscall.h"

int 
main()
{
	int i;
	char c;
	for (i = 32; i <= 126; ++i)
	{
		c = (char)i;
		PrintChar(c);
		PrintChar('\n');
	}
	Halt();
}
