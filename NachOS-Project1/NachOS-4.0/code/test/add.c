/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
  char* buffer;
  int result;
  ReadString(buffer, 20);
  result = Open("hello.txt", 1);
  PrintNum(result);
  Halt();
  /* not reached */
}
