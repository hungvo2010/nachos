#include "PTable.h"
#include "main.h"
#include "openfile.h"

PTable::PTable(int size){
    if (size < 0)
        return;

    this->psize = size;
    this->bm = new Bitmap(size);
    this->bmsem = new Semaphore('bmsem', 1);

    for(int i=0; i<psize; ++i){
        this->pcb[i] = NULL;
    }

    this->bm->Mark(0);

    // First process will have processId = 0, parentId of first process = -1
	this->pcb[0] = new PCB(0);
	// this->pcb[0]->SetFileName("./test/scheduler");
	this->pcb[0]->parentID = -1;
}

PTable::~PTable(){
    if( this->bm != 0 ){
        delete this->bm;
        this->bm = NULL;
    }
	    
    
    for(int i=0; i<psize; ++i)
		if(this->pcb[i] != 0){
            delete this->pcb[i];
            this->pcb[i] = NULL;
        }
			
    }
		
	if (bmsem != 0){
        delete this->bmsem;
        this->bmsem = NULL;
    }
}

int PTable::ExecUpdate(char* name){
    bmsem->P();
    if (name == NULL || strlen(name) == 0){
        bm->V();
        return -1;
    }
    if( strcmp(name,"./test/scheduler") == 0 || strcmp(name,currentThread->getName()) == 0 )
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
int PTable::JoinUpdate(int id){
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

int PTable::ExitUpdate(int exitcode)
{              
	int id = kernel->currentThread->processID;
	if(id == 0) //main process
	{
		
		kernel->currentThread->FreeSpace();		
		kernel->interrupt->Halt();
		return 0;
	}
    
    if(IsExist(id) == false)
	{
		printf("\nPTable::ExitUpdate: This %d is not exist. Try again?", id);
		return -1;
	}
	
	// Ngược lại gọi SetExitCode để đặt exitcode cho tiến trình gọi.
	pcb[id]->SetExitCode(exitcode);
	pcb[pcb[id]->parentID]->DecNumWait();
    
    // Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có) và ExitWait() để xin tiến trình cha
    // cho phép thoát.
	pcb[id]->JoinRelease();
    // 
	pcb[id]->ExitWait();
	
	Remove(id);
	return exitcode;
}

void PTable::Remove(int pid){
    // for(int i=0; i<psize; ++i){
    //     if (this->pcb[i]->GetID() == pid){
    //         delete this->pcb[i];
    //         this->pcb[i] = NULL;
    //         this->bm->Clear(i);
    //         return;
    //     }
    // }
    this->bm->Clear(pid);
    if(this->pcb[pid] != 0){
        delete this->pcb[pid];
        this->pcb[pid] = NULL;
    }
}

bool PTable::IsExist(int pid){
    return this->bm->Test(pid);
}

int PTable::GetFreeSlot(){
    return this->bm->FindAndSet();
}

char* PTable::GetFileName(int id){
    return this->pdb[id]->GetFileName();
}

PCB* PTable::GetPCB(int pid){
    return this->pdb[pid];
}