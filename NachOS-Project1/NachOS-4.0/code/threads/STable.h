// STable.h
// describe semaphore

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef STable_H
#define STable_H

#include "bitmap.h"
#include "Sem.h"

#define MAX_SEMAPHORE 10

class STable {
    private:
        Bitmap* bm;
        Sem* semTab[MAX_SEMAPHORE];
    public:
        STable();
        ~STable();
        int Create(char* name, int init);
        int Wait(char* name); 
        int Signal(char* name);
        int FindFreeSlot(int id);
};

#endif