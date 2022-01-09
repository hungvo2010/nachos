/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

void Sys_PrintNum(int n)
{
	if (n ==0)
	{
		kernel->synchConsoleOut->PutChar('0');
		return;
	}

	if (n < 0)
	{
		kernel->synchConsoleOut->PutChar('-');
		n = -n;
	}

	char buffer[10];
	int count;
	count = 0;
	while (n > 0)
	{
		buffer[count] = n%10 + '0';
		count++;
		n = n / 10;
	}
	for(; count > 0; count--)
	{
		kernel->synchConsoleOut->PutChar(buffer[count-1]);
	}
}

int Sys_ReadNum()
{
	// If the input number is exceeded the C limit, n will be overflowed.
	// Want to handle it? Use a variable to count number of input character, if it large than 9, we discard them and return 0
	int n = 0;
	int sign = 1;
	char s;
	s = kernel->synchConsoleIn->GetChar();
	if (s == '-')
	{
		n = -n;
		sign = -1;
	}
	else if (s < '0' || s > '9')
		return 0;
	else 
		n = n * 10 + (s - '0');
		
	do
	{
		s = kernel->synchConsoleIn->GetChar();
		if (s == '\n' || s == ' ')
			break;
		if (s < '0' || s > '9')
			return 0;
		n = n * 10 + (s - '0');
	} while (1)
	return sign * n;
}

char Sys_ReadChar()
{
	return kernel->synchConsoleIn->GetChar();
}

void Sys_PrintChar(char c)
{
	kernel->synchConsoleOut->PutChar(c);
}

int Sys_RandomNum()
{
	RandomInit(time(0));
	return RandomNumber();
}

void Sys_PrintString(int buffer_address, int length)
{
	int oneIntChar;
	char oneChar;
	int i;
	for (i = 0; i < length; ++i)
	{
		kernel->machine->ReadMem(buffer_address + i, 1, &oneIntChar);
		if (oneChar == 0)
			break;
		oneChar = (char)oneIntChar;
		kernel->synchConsoleOut->PutChar(oneChar);
	}
}

void Sys_ReadString(int buffer_address, int length)
{
	int oneIntChar;
	char oneChar;
	int i;
	for (i = 0; i < length; ++i)
	{
		oneChar = kernel->synchConsoleIn->GetChar();
		oneIntChar = (int)oneChar;
		if (oneChar == EOF || oneChar == '\n' || oneIntChar == 0)
			break;
		kernel->machine->WriteMem(buffer_address + i, 1, oneIntChar);
	}
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
