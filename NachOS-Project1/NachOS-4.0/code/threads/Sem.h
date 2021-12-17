// Sem.cc
// Manage semaphore

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "synch.h"
class Sem {
    private:
        char name[50];
        Semaphore *sem;
    public:
        Sem(){
            sem = NULL;
        }
        Sem(char* na, int i){
            strcpy(this->name,na);
            sem = new Semaphore(name,i);
        }
        ~Sem(){
            delete sem;
        }
        void wait(){
            sem->P();
        }
        void signal(){
            sem->V();
        }
        char* GetName(){
            return name;
        }
};