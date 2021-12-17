#include "PTable.h"
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

PCB PTable::GetPCB(int pid){
    return this->pdb[pid];
}