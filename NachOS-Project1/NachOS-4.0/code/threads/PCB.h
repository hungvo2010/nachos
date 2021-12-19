// PCB.h
// Process control block

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "Filetable.h"
#include "synch.h"

class PCB {
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

        int CreateFile(char* filename);
        OpenFileID OpenFile(char* filename, int type);
        int ReadFile(char* buffer, int charcount, OpenFileID id);
        int WriteFile(char* buffer, int charcount, OpenFileID id);
        int CloseFile(OpenFileID id);

    private:
        Semaphore* joinsem = NULL;             // semaphore for join process
        Semaphore* exitsem = NULL;             // semaphore for exit process
        Semaphore* multex = NULL;              // semaphore for unique access
        int exitcode;                   //    
        int numwait;                    // number of process joined
        Thread* thread;
        Filetable* filetable;
};

#endif // PCB_H