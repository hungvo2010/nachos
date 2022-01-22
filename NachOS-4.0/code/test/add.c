/* add.c
 *	Simple program to test add function.
 */

#include "syscall.h"

int
main()
{
  int result;
  
  result = Add(42, 23);

  Halt();
  /* not reached */
}
