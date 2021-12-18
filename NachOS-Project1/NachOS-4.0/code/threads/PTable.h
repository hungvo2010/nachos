// PTable.h
// Process table

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef PTABLE_H
#define PTABLE_H

#include "main.h"
#include "PCB.h"
#include "bitmap.h"
#include "synch.h"

#define MAX_PROCESS 10


class PTable {
    private:
        Bitmap* bm = NULL;
        PCB* pcb[MAX_PROCESS];
        int psize;
        Semaphore* bmsem = NULL;
    public:
        PTable(int size){
            if (size < 0)
                return;

            this->psize = size;
            this->bm = new Bitmap(size);
            this->bmsem = new Semaphore("bmsem", 1);

            for(int i=0; i<psize; ++i){
                this->pcb[i] = NULL;
            }

            this->bm->Mark(0);

            // First process will have processId = 0, parentId of first process = -1
            this->pcb[0] = new PCB(0);
            // this->pcb[0]->SetFileName("./test/scheduler");
            this->pcb[0]->parentID = -1;
        }
        ~PTable(){
            if( this->bm != NULL ){
                delete this->bm;
                this->bm = NULL;
            }
	    
    
            for(int i=0; i<psize; ++i){
                if(this->pcb[i] != 0){
                    delete this->pcb[i];
                    this->pcb[i] = NULL;
                }
			
            }
		
            if (bmsem != NULL){
                delete this->bmsem;
                this->bmsem = NULL;
            }
        }
        int ExitUpdate(int ec){
            int id = kernel->currentThread->processID;
            if(id == 0) //main process
            {
		
                kernel->currentThread->FreeSpace();		
                kernel->interrupt->Halt();
                return 0;
            }
    
            if(this->IsExist(id) == false)
            {
                printf("\nPTable::ExitUpdate: This %d is not exist. Try again?", id);
                return -1;
            }
	
            // Ngược lại gọi SetExitCode để đặt exitcode cho tiến trình gọi.
            this->pcb[id]->SetExitCode(ec);
            this->pcb[pcb[id]->parentID]->DecNumWait();
    
            // Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có) và ExitWait() để xin tiến trình cha
            // cho phép thoát.
            pcb[id]->JoinRelease();
            // 
            pcb[id]->ExitWait();
	
            Remove(id);
            return ec;
        } 
        int ExecUpdate(char* name){
            bmsem->P();
            if (name == NULL || strlen(name) == 0){
                bmsem->V();
                return -1;
            }
            if( strcmp(name,"./test/scheduler") == 0 || strcmp(name, kernel->currentThread->getName()) == 0 )
            {
                printf("\nPTable::Exec : Can't not execute itself.\n");		
                bmsem->V();
                return -1;
            }
            int index = this->GetFreeSlot();
            if(index < 0)
            {
                bmsem->V();
                return -1;
            }

            pcb[index] = new PCB(index);
            pcb[index]->SetFileName(name);
            pcb[index]->parentID = kernel->currentThread->processID;

            int pid = pcb[index]->Exec(name, index);

            bmsem->V();
            return pid;
        }
        int JoinUpdate(int id){
            if (id < 0){    // most parent thread
                printf("\nPTable::JoinUpdate : id = %d", id);
                return -1;
            }
            if (kernel->currentThread->processID != pcb[id]->parentID){
                return -1;
            }
            pcb[pcb[id]->parentID]->IncNumWait();
            pcb[id]->JoinWait();
            int ec = pcb[id]->GetExitCode();
            pcb[id]->ExitRelease();
            return ec;
        } 
        void Remove(int pid){
            this->bm->Clear(pid);
            if(this->pcb[pid] != 0){
                delete this->pcb[pid];
                this->pcb[pid] = NULL;
            }
        }
        bool IsExist(int pid){
            return this->bm->Test(pid);
        }

        int GetFreeSlot(){
            return this->bm->FindAndSet();
        }

        char* GetFileName(int id){
            return this->pcb[id]->GetFileName();
        }

        PCB* GetPCB(int pid){
            return this->pcb[pid];
        }

        // PTable(int size);
        // ~PTable();
        // int ExitUpdate(int ec); 
        // int ExecUpdate(char* name);
        // int JoinUpdate(int id); 
        // bool IsExist(int pid); 
        // int GetFreeSlot();
        // void Remove(int pid);
        // char* GetFileName(int id);
        // PCB GetPCB(int pid);
};

#endif // PTABLE_H