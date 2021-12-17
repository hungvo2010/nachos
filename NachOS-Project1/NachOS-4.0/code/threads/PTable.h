// PTable.h
// Process table

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef PTable_H
#define PTable_H

#include "PCB.h"
#include "bitmap.h"

#define MAX_PROCESS 10


class PTable {
    private:
        Bitmap bm;
        PCB* pcb[MAX_PROCESS];
        int psize;
        Semaphore* bmsem = NULL;
    public:
        PTable(int size);
        ~PTable();
        int ExitUpdate(int ec); 
        int ExecUpdate(char* name);
        int JoinUpdate(int id); 
        bool IsExist(int pid); 
        int GetFreeSlot();
        void Remove(int pid);
        char* GetFileName(int id);
        PCB GetPCB(int pid);
};

#endif