/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
  int result;
  // char* filename;
  // ReadString(filename, 30);
  // result = CreateFile(filename);
  result = Add(-4, 4);
  PrintNum(result);
  Halt();
  /* not reached */
}
