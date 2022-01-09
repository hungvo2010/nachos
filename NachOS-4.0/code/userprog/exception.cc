// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "synchconsole.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void IncPC()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}
#define MAX_FILE_LENGTH 50
void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{

	case NoException:
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
		return;

	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();
			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			int result;
			result = SysAdd((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();

			return;

		case SC_PrintNum:

			DEBUG(dbgSys, "PrintNum Exception " << kernel->machine->ReadRegister(4) << "\n");

			SysPrintNum((int)kernel->machine->ReadRegister(4));

			IncPC();
			return;

		case SC_ReadNum:

			DEBUG(dbgSys, "ReadNum Exception \n");

			result = SysReadNum();

			DEBUG(dbgSys, "ReadNum returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_PrintChar:

			DEBUG(dbgSys, "PrintChar Exception" << (char)kernel->machine->ReadRegister(4) << "\n");

			SysPrintChar((char)kernel->machine->ReadRegister(4));

			IncPC();
			return;

		case SC_ReadChar:

			DEBUG(dbgSys, "ReadChar Exception \n");

			result = SysReadChar();

			DEBUG(dbgSys, "ReadChar returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_RandomNum:

			DEBUG(dbgSys, "RandomNum Exception \n");

			result = SysRandomNum();

			DEBUG(dbgSys, "RandomNum returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_PrintString:

			DEBUG(dbgSys, "PrintString Exception \n");

			int buffer_address;
			buffer_address = (int)kernel->machine->ReadRegister(4);
			int buffer_length;
			buffer_length = (int)kernel->machine->ReadRegister(5);

			DEBUG(dbgSys, "PrintString Exception with buffer_address: " << buffer_address << ", buffer length: " << buffer_length << "\n");
			SysPrintString(buffer_address, buffer_length);

			IncPC();
			return;

		case SC_ReadString:

			DEBUG(dbgSys, "ReadString Exception \n");

			buffer_address = (int)kernel->machine->ReadRegister(4);
			buffer_length = (int)kernel->machine->ReadRegister(5);

			DEBUG(dbgSys, "ReadString Exception with buffer_address: " << buffer_address << ", buffer length: " << buffer_length << "\n");
			SysReadString(buffer_address, buffer_length);

			IncPC();
			return;

		case SC_Create:

			DEBUG(dbgSys, "Create file Exception \n");
			int filename_address;
			filename_address = (int)kernel->machine->ReadRegister(4);

			char *filename;
			filename = User2System(filename_address, MAX_FILE_LENGTH);
			DEBUG(dbgSys, "Create file Exception : " << filename_address << "\n");

			result = SysCreatefile(filename);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Open:

			DEBUG(dbgSys, "Open file Exception \n");
			filename_address = (int)kernel->machine->ReadRegister(4);
			int type;
			type = (int)kernel->machine->ReadRegister(5);

			filename = User2System(filename_address, MAX_FILE_LENGTH);

			result = SysOpenfile(filename, type);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Read:

			DEBUG(dbgSys, "Read file Exception \n");
			int file_id;
			char *buffer;
			buffer_address = (int)kernel->machine->ReadRegister(4);
			buffer_length = (int)kernel->machine->ReadRegister(5);
			file_id = (int)kernel->machine->ReadRegister(6);

			result = SysReadfile(file_id, buffer_address, buffer_length);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Write:

			DEBUG(dbgSys, "Open file Exception \n");
			buffer_address = (int)kernel->machine->ReadRegister(4);
			buffer_length = (int)kernel->machine->ReadRegister(5);
			file_id = (int)kernel->machine->ReadRegister(6);

			result = SysWritefile(file_id, buffer_address, buffer_length);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Close:

			DEBUG(dbgSys, "Close file Exception \n");

			file_id = (int)kernel->machine->ReadRegister(4);
			DEBUG(dbgSys, "Close file Exception : " << file_id << "\n");

			result = SysClosefile(file_id);
			kernel->machine->WriteRegister(2, (int)result);
			
			IncPC();
			return;

		case SC_Exec:
			DEBUG(dbgSys, "Exec file Exception \n");
			filename_address = (int)kernel->machine->ReadRegister(4);
			filename = User2System(filename_address, MAX_FILE_LENGTH);
			result = SysExec(filename);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Join:
			DEBUG(dbgSys, "Join Exception \n");
			int processID;
			processID = (int)kernel->machine->ReadRegister(4);
			DEBUG(dbgSys, "Join Exception: " << processID << "\n");
			result = kernel->pTab->JoinUpdate(processID);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_Exit:
			DEBUG(dbgSys, "Exit Exception \n");
			int exit_code;
			exit_code = (int)kernel->machine->ReadRegister(4);
			result = kernel->pTab->ExitUpdate(exit_code);
			kernel->machine->WriteRegister(2, (int)result);

			IncPC();
			return;

		case SC_CreateSemaphore:
			DEBUG(dbgSys, "Create Semaphore Exception \n");
			int semaphoreName_addr, semaphoreVal;
			semaphoreName_addr = (int)kernel->machine->ReadRegister(4);
			semaphoreVal = (int)kernel->machine->ReadRegister(5);

			char *semaphore_name;
			semaphore_name = User2System(semaphoreName_addr, MAX_FILE_LENGTH);
			if (semaphore_name == NULL)
				result = -1;
			else
				result = kernel->semTab->Create(semaphore_name, semaphoreVal);
			kernel->machine->WriteRegister(2, (int)result);
			if (semaphore_name)
			{
				delete[] semaphore_name;
				semaphore_name = 0;
			}
			IncPC();
			return;

		case SC_Signal:
			DEBUG(dbgSys, "Up Exception \n");
			/* Process Create file system call*/
			semaphoreName_addr = (int)kernel->machine->ReadRegister(4);
			semaphore_name = User2System(semaphoreName_addr, MAX_FILE_LENGTH);

			if (semaphore_name == NULL)
				result = -1;
			else
				result = kernel->semTab->Signal(semaphore_name);

			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
			if (semaphore_name)
			{
				delete[] semaphore_name;
				semaphore_name = 0;
			}
			IncPC();
			return;

		case SC_Wait:
			DEBUG(dbgSys, "Up Exception \n");
			/* Process Create file system call*/
			semaphoreName_addr = (int)kernel->machine->ReadRegister(4);
			semaphore_name = User2System(semaphoreName_addr, MAX_FILE_LENGTH);

			if (semaphore_name == NULL)
				result = -1;
			else
				result = kernel->semTab->Wait(semaphore_name);
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
			if (semaphore_name)
			{
				delete[] semaphore_name;
				semaphore_name = 0;
			}
			IncPC();
			return;


		case SC_GetPID:
			result = kernel->currentThread->processID;

			kernel->machine->WriteRegister(2, (int)result);
			IncPC();
			return;

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
            DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
            SysHalt();
            break;
            return;
        }
	ASSERTNOTREACHED();
}
