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
  int file;
  ReadString(filename, 10);
  CreateFile(filename);
  file = Open(filename, 0);
  ReadString(content, 10);
  result = Write(content, 40, file);
  result = Read(readcontent, 40, file);
  PrintNum(result); // will be -2? write make file pointer to end of file, read at end of file return -2.
  Halt();
  /* not reached */
}
