/* printchar.c
 *	Test rogram read a character from console, then print it back to console.
 *	
 *	Do ReadChar and PrintChar syscall.
 *
 */

#include "syscall.h"

int main()
{
	char s;
	s = ReadChar();
	PrintChar(s);
	Halt();
}
