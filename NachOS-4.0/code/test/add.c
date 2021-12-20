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
  PrintNum(file);
  ReadString(content, 10);
  PrintString(content);
  result = Write(content, 40, file);
  PrintNum(result);
  result = Read(readcontent, 40, file);
  PrintString("\n-----\n");
  PrintNum(result);
  PrintString(readcontent);
  Halt();
  /* not reached */
}
