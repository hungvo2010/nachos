/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
  char* filename = "hello.txt";
  int result = Create(filename);
  PrintNum(result);
  Halt();
  /* not reached */
}
