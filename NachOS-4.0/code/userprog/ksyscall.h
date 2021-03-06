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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kernel.h"
#include "main.h"
#include "pcb.h"
#include "synchconsole.h"
#define MAX_FILE 10

char *User2System(int virtAddr, int limit);
int System2User(int virtAddr, int len, char *buffer);
bool WillOverflow(int cur, int next);
void SysPrintNum(int number);
void SysHalt() { kernel->interrupt->Halt(); }

int SysAdd(int op1, int op2) { return op1 + op2; }

int SysExec(int virAddr) {
    char *name = User2System(virAddr, 255);
    if (name == NULL) {
        return -1;
    }
    OpenFile *file = kernel->fileSystem->Open(name);
    if (file == NULL) {
        return -1;
    }
    delete file;
    int pid = kernel->pTab->ExecUpdate(name);
    return pid;
}

int SysJoin(int id) {
    int res = kernel->pTab->JoinUpdate(id);
    return res;
}

int SysCreateSemaphore(int virAddr, int semval) {
    char *name = User2System(virAddr, 255);
    if (name == NULL || strlen(name) == 0) {
        return -1;
    }
    int result = kernel->semTab->Create(name, semval);
    return result;
}

int SysWait(int viraddr) {
    char *name = User2System(viraddr, 255);
    if (name == NULL || strlen(name) == 0) {
        return -1;
    }
    int result = kernel->semTab->Wait(name);
    return result;
}

int SysSignal(int viraddr) {
    char *name = User2System(viraddr, 255);
    if (name == NULL || strlen(name) == 0) {
        return -1;
    }
    int result = kernel->semTab->Signal(name);
    return result;
}

// Cai dat cua ham CreateFile, duoc goi trong exception.cc
int SysCreateFile(int virAddr) {
    char *filename = User2System(virAddr, 255);
    if (filename == NULL || strlen(filename) == 0) {
        printf("%s", "File name is not valid");
        return -1;
    }
    if (filename == NULL)
        return -1;
    return kernel->fileSystem->Create(filename) ? 0 : -1;
}

// Cai dat cua ham OpenFile, duoc goi trong exception.cc
int SysOpenFile(int virAddr, int type) {
    char *filename = User2System(virAddr, 255);
    if (filename == NULL || strlen(filename) == 0) {
        printf("%s", "File name is not valid");
        return -1;
    }

    // check file open mode is valid, 0: read and write, 1: read only
    // if (type != 0 && type != 1) {
    //     printf("%s", "Open file mode is not valid");
    //     return -1;
    // }

    int processId = kernel->currentThread->processID;
    PCB *curProccess = kernel->pTab->GetProcess(processId);

    if (kernel->fileSystem->isFileTableFull() || curProccess->isFileTableFull()) {
        printf("%s", "Directory is full or process file is not enough");
        return -1;
    }
    OpenFile *newfile = kernel->fileSystem->Open(filename, type);
    if (!newfile) return -1;
    int result = curProccess->Open(newfile);
    return result;
}

// Cai dat cua ham CloseFile, duoc goi trong exception.cc
int SysCloseFile(int id) {
    // id < 0: not valid, id = 0: output to screen, id = 1: read from keyboard, id >= MAX_FILE: not valid
    if (id < 2 || id >= MAX_FILE) {
        return -1;
    }

    int processId = kernel->currentThread->processID;
    PCB *curProccess = kernel->pTab->GetProcess(processId);
    OpenFile *closeFile = curProccess->Close(id);
    if (!closeFile) return -1;
    return kernel->fileSystem->Close(closeFile) ? 0 : -1;
}

int SysReadNum() {
    int res = 0,
        sign = 1;  // res la ket qua tra ve, sign la dau (1: duong, -1: am)
    char ch;       // ky tu dang duoc doc
    ch = kernel->synchConsoleIn
             ->GetChar();  // doc ky tu dau tien de xem so am hay duong
    if (ch == '-') {
        sign = -1;
    } else if (ch == '+') {
        sign = 1;
    } else if (ch < '0' || ch > '9') {  // neu khong phai so thi tra ve 0
        return 0;
    } else {
        res = ch - '0';  // neu khong phai so am thi luw ket qua
    }

    while (true) {
        ch = kernel->synchConsoleIn->GetChar();  /// doc toan bo chuoi con lai
        if (ch == '\n') {                        // neu la enter, luu gia tri duoc nhap
            return res * sign;
        }
        if (ch < '0' || ch > '9') {
            return 0;
        }
        if (!WillOverflow(res, ch - '0')) {  // neu khong tran so, tinh gia tri
            // den vi tri dang doc
            res = res * 10 + (ch - '0');
        } else {  // new tran so, tra ve 0
            return 0;
        }
    }
}

bool WillOverflow(int cur, int next) {
    // Xet tran so, neu > INT_MAX hoac < INT_MIN thi tran so
    return (((INT_MAX - next) / 10) < cur || (INT_MIN + next) / 10 > cur);
}

// Cai dat cua ham PrintNum, duoc goi trong exception.cc
void SysPrintNum(int number) {
    char str[20];
    int i = 0;
    sprintf(str, "%d", number);  // Bien so thanh chuoi
    while (str[i] != '\0') {     // in ra tung ky tu cua chuoi
        kernel->synchConsoleOut->PutChar(str[i]);
        i++;
    }
}

// Cai dat cua ham ReadChar, duoc goi trong exception.cc
char SysReadChar() {
    return kernel->synchConsoleIn->GetChar();
}  // doc ky tu tu console

// Cai dat cua ham PrintChar, duoc goi trong exception.cc
void SysPrintChar(char character) {
    kernel->synchConsoleOut->PutChar(character);  // in ky tu ra man hinh
}

// Cai dat cua ham RandomNum, duoc goi trong exception.cc
int SysRandomNum() {
    srand(time(0));
    return rand();
}

// Cai dat cua ham ReadString, duoc goi trong exception.cc
int SysReadString(int virtAddress, int length) {
    char *buffer;
    int i = 0;
    char ch;
    bool flag = false;
    buffer = User2System(virtAddress, length);
    for (i = 0; i < length; ++i) {
        buffer[i] = 0;
    }
    i = 0;
    while (i < length && flag == false) {
        do {
            ch = kernel->synchConsoleIn->GetChar();
        } while (ch == EOF);
        if (ch == '\012' || ch == '\001') {
            flag = true;
        } else {
            buffer[i++] = ch;
        }
    }
    System2User(virtAddress, length, buffer);  // chuyen vung nho ve lai user-space
    delete buffer;
    return i;
}

// Ham copy buffer tu user-space vao kernel-space
char *User2System(int virtAddr, int limit) {
    int i;  // index
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit + 1];  // need for terminal string
    if (kernelBuf == NULL)
        return kernelBuf;

    memset(kernelBuf, 0, limit + 1);

    for (i = 0; i < limit; i++) {
        kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
        kernelBuf[i] = (char)oneChar;
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

// Cai dat cua ham PrintString, duoc goi trong exception.cc
int SysPrintString(int virtAdrr) {
    int i = 0;
    char *buffer;
    buffer = User2System(virtAdrr, 1000);  // chuyen du lieu tu user-space vao kernel-space
    while (buffer[i] != '\0') {            // in tung ky tu ra man hinh
        kernel->synchConsoleOut->PutChar((char)buffer[i]);
        i++;
    }
    delete buffer;
    return i;
}

// Ham copy buffer tu kernel-space ra user-space
int System2User(int virtAddr, int len, char *buffer) {
    if (len < 0)
        return -1;
    if (len == 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do {
        oneChar = (int)buffer[i];
        kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}

// Cai dat cua ham ReadFile, duoc goi trong exception.cc
int SysReadFile(int virAddr, int charcount, int id) {
    if (charcount < 0 || id == 1 || id >= MAX_FILE) {
        return -1;
    }

    // read from keyboard/console
    if (id == 0) {
        int numchar = SysReadString(virAddr, charcount);
        return numchar;
    }

    char *buffer = new char[charcount + 1];

    int processId = kernel->currentThread->processID;
    PCB *curProccess = kernel->pTab->GetProcess(processId);
    OpenFile *file = curProccess->GetFile(id);
    if (!file) return -1;

    int charRead = file->Read(buffer, charcount);
    System2User(virAddr, charRead, buffer);
    delete[] buffer;
    return charRead;
}

int SysWriteFile(int virAddr, int charcount, int id) {
    if (charcount < 0 || id <= 0 || id >= MAX_FILE) {
        return -1;
    }

    // write to screen/console
    if (id == 1) {
        int result = SysPrintString(virAddr);
        return result;
    }
    char *buffer = User2System(virAddr, charcount);
    int len = strlen(buffer);
    int processId = kernel->currentThread->processID;
    PCB *curProccess = kernel->pTab->GetProcess(processId);
    OpenFile *file = curProccess->GetFile(id);
    int charWrite = file->Write(buffer, len);
    return charWrite;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
