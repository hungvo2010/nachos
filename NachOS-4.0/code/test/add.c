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
  char* filename;
  char* content;
  char* readcontent;
  ReadString(filename, 30);
  CreateFile(filename);
  result = Open(filename, 0);
  ReadString(content, 30);
  Write(content, 40, result);
  Read(readcontent, 40, result);
  PrintString(readcontent);
  Halt();
  /* not reached */
}
