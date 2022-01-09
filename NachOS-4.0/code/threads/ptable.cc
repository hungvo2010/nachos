#include "ptable.h"

PTable::PTable(int size) {
    if (size < 0)
        return;

    this->psize = size;
    this->bm = new Bitmap(size);
    this->bmsem = new Semaphore("bmsem", 1);

    pcb[0] = new PCB(0);
    bm->Mark(0);
}

PTable::~PTable() {
    delete bm;
    delete bmsem;
    for (int i = 0; i < psize; ++i) {
        if (pcb[i])
            delete pcb[i];
    }
}
/*
return PID
Thực thi cho system call SC_EXEC, 
kiểm tra chương trình được gọi có tồn tại trong máy không. 
Kiểm tra thử xem chương trình gọi lại chính nó không? 
Chúng ta không cho phép điều này. 
Kiểm tra còn slot trống để lưu tiến trình mới không (max là 10 process). 
Nếu thỏa các điều kiện trên thì ta lấy index của slot trống
 là processID cảu tiền trình mới tạo này, giả sử là ID
 . Và gọi phương thức EXEC của lớp PCB với đối tượng tương ứng quản lý 
 process này, nghĩa là gọi pcb[ID]->Exec(…). 
 Xem chi tiết mo tả trong lớp PCB ở bên dưới.
*/

int PTable::ExecUpdate(char *name) {
    bmsem->P();
    if (name == NULL) {
        bmsem->V();
        return -1;
    }

    OpenFile *executable = kernel->fileSystem->Open(name, 1);

    if (executable == NULL) {
        bmsem->V();
        return -1;
    }
    delete executable;

    if (!strcmp(kernel->currentThread->getName(), name)) {
        bmsem->V();
        return -1;
    }

    int processID;
    if (bm->NumClear() == 0) {
        bmsem->V();
        return -1;
    }

    if (bm->NumClear() == psize) {
        processID = 0;
        bm->Mark(processID);
    } else {
        processID = bm->FindAndSet();
    }
    bmsem->V();
    pcb[processID] = new PCB(processID);

    return pcb[processID]->Exec(name, processID);
}

int PTable::JoinUpdate(int childpid) {
    if (childpid < 0 || childpid >= MAX_PROCESS)
        return -1;
    if (bm->Test(childpid) == 0)
        return -1;

    int processID = kernel->currentThread->processID;

    if (processID != pcb[childpid]->parentID)
        return -1;
    pcb[processID]->IncNumWait();
    pcb[processID]->JoinWait();

    int exitcode = pcb[childpid]->GetExitCode();
    pcb[childpid]->ExitRelease();
    return exitcode;
}

int PTable::ExitUpdate(int exitcode) {
    int processID = kernel->currentThread->processID;
    int parentpID = pcb[processID]->parentID;

    pcb[processID]->SetExitCode(exitcode);

    if (processID == 0) {
        bmsem->V();
        kernel->interrupt->Halt();
    }

    pcb[parentpID]->JoinRelease();
    pcb[processID]->ExitWait();
    bm->Clear(processID);
    delete addrspace[processID];

    kernel->currentThread->Finish();
    if (pcb[processID])
        delete pcb[processID];

    return exitcode;
}

int PTable::GetFreeSlot() {
    if (bm->NumClear() == psize)
        return -1;
    else
        return bm->FindAndSet();
}

bool PTable::IsExist(int pid) {
    return bm->Test(pid);
}

void PTable::Remove(int pid) {
    bm->Clear(pid);
    delete pcb[pid];
}
//Xóa một processID ra khỏng mãng quản lý nó, khi mà tiến trình này kết thúc.

char *PTable::GetFileName(int id) {
    return pcb[id]->getThread()->getName();
}  // Trả về tên của tiến trình

PCB *PTable::GetProcess(int id) {
    if (bm->Test(id))
        return pcb[id];
    else
        return NULL;
}