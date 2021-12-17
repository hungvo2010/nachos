// PCB.h
// Process control block

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef PCB_H
#define PCB_H

#include "synch.h"

class PCB {
    private:
        Semaphore* joinsem;             // semaphore for join process
        Semaphore* exitsem;             // semaphore for exit process
        Semaphore* multex;              // semaphore for unique access
        int exitcode;                   //    
        int numwait;                    // number of process joined
        Thread* thread;
    public:
        int parentID;        
    PCB();
    PCB(int id);
    ~PCB();

    int Exec(char *filename, int pid);
    int GetID();
    int GetNumWait(); 
    void IncNumWait();
    void DecNumWait(); 
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void SetExitCode(int ec);
    int GetExitCode();
    void SetFileName(char* fn);
    char* GetFileName();
};

#endif