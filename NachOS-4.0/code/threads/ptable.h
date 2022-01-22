#ifndef PTABLE_H
#define PTABLE_H
#include "bitmap.h"
#include "pcb.h"
#include "synch.h"
#define MAX_PROCESS 10
class Semaphore;
class PCB;

class PTable {
   private:
    Bitmap* bm;

    PCB* pcb[MAX_PROCESS];
    int psize;
    Semaphore* bmsem;  // Semaphore nay giup ngan 2 tien trinh cung nap 1 luc
   public:
    AddrSpace* addrspace[MAX_PROCESS];
    PTable(int size);

    ~PTable();

    int ExecUpdate(char* name);

    int ExitUpdate(int ec);

    int JoinUpdate(int id);

    int GetFreeSlot();

    bool IsExist(int pid);

    void Remove(int pid);

    char* GetFileName(int id);

    PCB* GetProcess(int id);
};
#endif
