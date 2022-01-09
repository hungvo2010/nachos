/* readstring.c
 *	Test program read a string from console, then print it back to console.
 *	
 *	Do ReadString and PrintString syscall.
 *
 */

#include "syscall.h"

int
main()
{
	char buffer[100];
	ReadString(buffer, 100);
	PrintString(buffer, 100);
	Halt();
}
