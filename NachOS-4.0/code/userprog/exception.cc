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
#include "ksyscall.h"
#include "main.h"
#include "synchconsole.h"
#include "syscall.h"
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
void IncPCReg() {
    /* set previous programm counter (debugging only)*/
    kernel->machine->WriteRegister(PrevPCReg,
                                   kernel->machine->ReadRegister(PCReg));

    /* set programm counter to next instruction (all Instructions are 4 byte
     * wide)*/
    kernel->machine->WriteRegister(PCReg,
                                   kernel->machine->ReadRegister(PCReg) + 4);

    /* set next programm counter for brach execution */
    kernel->machine->WriteRegister(NextPCReg,
                                   kernel->machine->ReadRegister(PCReg) + 4);
}

void ExceptionHandler(ExceptionType which) {
    int type = kernel->machine->ReadRegister(2);
    int result;
    char charResult;

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
        case NoException:
            /* set previous programm counter (debugging only)*/
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
            /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
            kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
            /* set next programm counter for brach execution */
            kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
            return;

        case SyscallException:
            switch (type) {
                case SC_Halt:
                    DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

                    SysHalt();
                    ASSERTNOTREACHED();
                    break;

                case SC_Add:  // Co san
                    DEBUG(dbgSys,
                          "Add " << kernel->machine->ReadRegister(4) << " + "
                                 << kernel->machine->ReadRegister(5) << "\n");

                    result = SysAdd(
                        (int)kernel->machine->ReadRegister(4),
                        (int)kernel->machine->ReadRegister(5));

                    DEBUG(dbgSys, "Add returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int)result);
                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_ReadNum:  // Syscall nay de doc so nguyen
                    DEBUG(dbgSys, "Read integer from console input\n");

                    result = SysReadNum();

                    DEBUG(dbgSys, "Read returning with " << result << "\n");

                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_PrintNum:  // Syscall in so nguyen
                    DEBUG(dbgSys, "Print " << kernel->machine->ReadRegister(4)
                                           << " to console output\n");

                    SysPrintNum((int)kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "Print completed\n");

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_ReadChar:
                    DEBUG(dbgSys, "Read char from console input\n");

                    charResult = SysReadChar();

                    DEBUG(dbgSys, "Read returning with " << charResult << "\n");

                    kernel->machine->WriteRegister(2, (int)charResult);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_PrintChar:
                    DEBUG(dbgSys, "Print " << kernel->machine->ReadRegister(4)
                                           << " to console output\n");

                    SysPrintChar((char)kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "Print completed\n");

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_RandomNum:  // Syscall nay de tao so nguyen ngau nhien
                    DEBUG(dbgSys, "Random number");

                    result = SysRandomNum();

                    DEBUG(dbgSys, "Random number: " << result << "\n");

                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_ReadString:
                    DEBUG(dbgSys, "Read string from console input\n");

                    SysReadString(kernel->machine->ReadRegister(4),
                                  kernel->machine->ReadRegister(5));

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_PrintString:  // Syscall nay de in chuoi ra console
                    DEBUG(dbgSys, "Print " << kernel->machine->ReadRegister(4)
                                           << " to console output\n");

                    SysPrintString(kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "Print completed\n");

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;

                case SC_CreateFile:  // Syscall nay de tao file
                {
                    DEBUG(dbgSys,
                          "CreateFile " << kernel->machine->ReadRegister(4) << "\n");


                    result = SysCreateFile(kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "CreateFile returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Open: { //Syscall nay de mo file
                    DEBUG(dbgSys,
                          "OpenFile " << kernel->machine->ReadRegister(4) << "\n"
                                      << kernel->machine->ReadRegister(5));
                    result = SysOpenFile(kernel->machine->ReadRegister(4), kernel->machine->ReadRegister(5));

                    DEBUG(dbgSys, "OpenFile returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Read: { // Syscall nay de doc file
                    result = SysReadFile(kernel->machine->ReadRegister(4),
                                         kernel->machine->ReadRegister(5), kernel->machine->ReadRegister(6));

                    DEBUG(dbgSys, "ReadFile returning with " << result << "\n");

                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Write: { // Syscall nay de ghi file
                    result = SysWriteFile(kernel->machine->ReadRegister(4),
                                          kernel->machine->ReadRegister(5), kernel->machine->ReadRegister(6));

                    DEBUG(dbgSys, "WriteFile returning with " << result << "\n");

                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Close: { // Syscall nay de dong file
                    result = SysCloseFile(kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "CloseFile returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Exec: { // Syscall nay de goi thuc thi chuong trinh con
                    result = SysExec(kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "Exec returning with " << result << "\n");

                    kernel->machine->WriteRegister(2, (int)result);


                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Join: { // Syscall nay de join chuong trinh con vao cha
                    result = SysJoin(kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "Join returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Exit: { // Syscall nay de xac nhan chuong trinh con duoc ngung thanh cong
                    DEBUG(dbgSys, "Exit Exception \n");
                    result = (int)kernel->machine->ReadRegister(4);
                    result = kernel->pTab->ExitUpdate(result);
                    /* Prepare Result */
                    kernel->machine->WriteRegister(2, (int)result);

                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_CreateSemaphore: { // Syscall nay tao ra cau truc semaphore moi
                    DEBUG(dbgSys, "Create Semaphore Exception \n");
                    result = SysCreateSemaphore(kernel->machine->ReadRegister(4),
                                                kernel->machine->ReadRegister(5));

                    DEBUG(dbgSys, "CreateSemaphore returning with " << result << "\n");
                    /* Prepare Result */
                    kernel->machine->WriteRegister(2, (int)result);

                    /* Modify return point */
                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Wait: {
                    result = SysWait(kernel->machine->ReadRegister(4));
                    DEBUG(dbgSys, "Wait returning with " << result << "\n");
                    /* Prepare Result */
                    kernel->machine->WriteRegister(2, (int)result);

                    /* Modify return point */
                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Signal: {
                    result = SysSignal(kernel->machine->ReadRegister(4));
                    DEBUG(dbgSys, "Wait returning with " << result << "\n");
                    /* Prepare Result */
                    kernel->machine->WriteRegister(2, (int)result);

                    /* Modify return point */
                    IncPCReg();

                    return;

                    ASSERTNOTREACHED();
                    break;
                }

                case SC_GetPID: { // Syscall lay id cua tien trinh
                    result = kernel->currentThread->processID;

                    kernel->machine->WriteRegister(2, (int)result);
                    IncPCReg();
                    return;
                }
                default:
                    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
                    SysHalt();
                    break;
                    return;
            }
            break;
        default:
            cerr << "Unexpected user mode exception" << (int)which << "\n";
            break;
    }
    ASSERTNOTREACHED();
}