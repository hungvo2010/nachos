
#include "ksyscall.h"
#include "main.h"
#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>

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
	if (n == 0)
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
		buffer[count] = n % 10 + '0';
		count++;
		n = n / 10;
	}
	for (; count > 0; count--)
	{
		kernel->synchConsoleOut->PutChar(buffer[count - 1]);
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
	} while (1);
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

int Sys_PrintString(int buffer_address, int length)
{
	char* s = User2System(buffer_address, length);
	if (s == NULL) return -1;
	int i = 0;
	while (s[i])
	{
		kernel->synchConsoleOut->PutChar(s[i]);
		i++;
	}
	if (s) delete[] s;
	return i;

	/*
	int oneIntChar;
	char oneChar;
	int i;
	for (i = 0; i < length; ++i)
	{
		kernel->machine->ReadMem(buffer_address + i, 1, &oneIntChar);
		if (oneIntChar == 0) // End of string
			break;
		oneChar = (char)oneIntChar;
		kernel->synchConsoleOut->PutChar(oneChar);
	}
	*/
}

int Sys_ReadString(int buffer_address, int length)
{
	int oneIntChar;
	char oneChar;
	int i;
	for (i = 0; i < length - 1; ++i)
	{
		oneChar = kernel->synchConsoleIn->GetChar();
		oneIntChar = (int)oneChar;
		if (oneChar == EOF) 
		{
			kernel->machine->WriteMem(buffer_address + i, 1, 0);
			return -2;
		}
		if (oneChar == '\n' || oneIntChar == 0)
			break;
		kernel->machine->WriteMem(buffer_address + i, 1, oneIntChar);
	}

	// Add '\0' to end of string
	kernel->machine->WriteMem(buffer_address + i, 1, 0);
	return i;
}

int Sys_Createfile(char* filename)
{
	if (filename == NULL)
		return -1;
	return kernel->fileSystem->Create(filename) ? 0 : -1;
}

int Sys_Openfile(char* filename, int type)
{
	if (type < 0 || type > 2) // 0 = read write; 1 = read only, 2 = append only
	{
		cout << "Syscall OpenFile argument is not vailid: type = " << type << "\n";
		return -1;
	}
	if (filename == NULL)
	{
		cout << "Syscall OpenFile argument is not vailid: filename = NULL \n";
		return -1;
	}
	int current_process_ID = kernel->currentThread->processID;
	if (kernel->fileSystem->isFileTableFull() || kernel->pTab->getProcess(current_process_ID)->isFileTableFull())
	{
		cout << "File table is full!\n";
		return -1;
	}
	// request kernel open a file
	OpenFile* newfile = kernel->fileSystem->Open(filename, type);
	if (newfile == NULL)
	{
		cout << "Cannot open file!\n";
		return -1;
	}
	int file_id_in_process = kernel->pTab->getProcess(current_process_ID)->Open(newfile);
	return file_id_in_process;
}

int Sys_Closefile(int file_id)
{
	if (file_id < 2)
		return -1;
	int current_process_ID = kernel->currentThread->processID;
	OpenFile* close_file = kernel->pTab->getProcess(current_process_ID)->Close(file_id);
	if (close_file == NULL) return -1;
	return kernel->fileSystem->Close(close_file) ? 0 : -1;
}

int Sys_Readfile(int file_id, int virtual_address, int size)
{
	if (file_id == 1) return -1;
	int num_writen;
	if (file_id == 0) 
	{
		num_writen = Sys_ReadString(virtual_address, size + 1);
		return num_writen;
	}

	int current_process_ID = kernel->currentThread->processID;
	OpenFile* file = kernel->pTab->getProcess(current_process_ID)->GetFile(file_id);
	if (file == NULL) return -1; 

	char* buffer = new char[size+1];
	for (int i = 0; i <= size; ++i) buffer[i] = 0;

	int num_read = file->Read(buffer, size);
	System2User(virtual_address, num_read, buffer);

	delete buffer;
	return num_read;
}

int Sys_Writefile(int file_id, int virtual_address, int size)
{
	if (file_id == 0) return -1;
	int num_writen;
	if (file_id == 1) 
	{
		num_writen = Sys_PrintString(virtual_address, size);
		return num_writen;
	}
	int current_process_ID = kernel->currentThread->processID;
	OpenFile* file = kernel->pTab->getProcess(current_process_ID)->GetFile(file_id);
	if (file == NULL) return -1;
	char* buffer = User2System(virtual_address, size);
	int buffer_len = strlen(buffer);
	num_writen = file->Write(buffer, buffer_len);
	if (buffer) delete[] buffer;
	return num_writen;
}

/*
Input: - User space address (int)
 - Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/
char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;

	kernelBuf = new char[limit + 1]; //need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;

	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;

		if (oneChar == 0)
			break;
	}
	kernelBuf[i] = 0;
	return kernelBuf;
}
/*
Input: - User space address (int)
 - Limit of buffer (int)
 - Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User memory space
*/
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

int Sys_Exec(char* filename)
{
	if (filename == NULL) return -1;
	return kernel->pTab->ExecUpdate(filename);
}

