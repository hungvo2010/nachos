#include "pcb.h"

PCB::PCB(int id) {
    pid = id;
    this->joinsem = new Semaphore("joinsem", 0);
    this->exitsem = new Semaphore("exitsem", 0);
    this->multex = new Semaphore("multex", 1);

    this->filetable[0] = kernel->fileSystem->filetable[0];
    this->filetable[1] = kernel->fileSystem->filetable[1];

    this->fileBitmap = new Bitmap(MAX_FILE);
    this->fileBitmap->Mark(0);
    this->fileBitmap->Mark(1);
}
PCB::~PCB() {
    if (joinsem != NULL)
        delete this->joinsem;
    if (exitsem != NULL)
        delete this->exitsem;
    if (multex != NULL)
        delete this->multex;
    if (fileBitmap) delete fileBitmap;

    if (thread) delete thread;
}

void PCB::JoinWait() {
    //Goi joinsem->P() sau do cho JoinRelease() de chay tiep
    joinsem->P();
}

void PCB::JoinRelease() {
    // Goi joinsem->V() de giai phong tien trinh da goi JoinWait()
    joinsem->V();
}

void PCB::ExitWait() {
    // Goi exitsem-->V() ngung tien trinh, cho ExitRelease()
    exitsem->P();
}

void PCB::ExitRelease() {  
    // Goi exitsem-->V() de free tien trinh dang cho duoc giai phong
    exitsem->V();
}

void PCB::IncNumWait() {
    // Tang so luong tien trinh dang cho
    multex->P();
    numwait++;
    multex->V();
}
void PCB::DecNumWait() {
    // Giam so luong tien trinh dang cho
    multex->P();
    numwait--;
    multex->V();
}
void PCB::SetExitCode(int ec) { exitcode = ec; }
int PCB::GetExitCode() { return exitcode; }

void StartProcess_2(int id);
int PCB::Exec(char* filename, int id) {
    // Goi multex->P() ngan khong cho 2 tien trinh chay cung luc
    multex->P();

    this->thread = new Thread(filename);

    if (this->thread == NULL) {
        printf("\nPCB::Exec: Not enough memory!\n");
        multex->V();
        return -1;
    }

    this->thread->processID = id;
    this->parentID = kernel->currentThread->processID;
    this->thread->Fork((VoidFunctionPtr)StartProcess_2, (void*)id);

    multex->V();
    return id;
}

void StartProcess_2(int id) {
    char* fileName = kernel->pTab->GetFileName(id);

    AddrSpace* space;
    space = new AddrSpace();
    if (space == NULL) return;
    space->Load(fileName);

    if (space == NULL) {
        printf("\nPCB::Exec: Can't create AddSpace.");
        return;
    }
    kernel->pTab->addrspace[id] = space;
    space->Execute();
}

bool PCB::isFileTableFull() {
    return fileBitmap->NumClear() == 0;
}

int PCB::Open(OpenFile* new_file) {
    if (isFileTableFull()) return -1;
    int id = fileBitmap->FindAndSet();
    filetable[id] = new_file;
    return id;
}

OpenFile* PCB::Close(int id) {
    if (!fileBitmap->Test(id)) return NULL;
    OpenFile* file = filetable[id];
    fileBitmap->Clear(id);
    filetable[id] = NULL;
    return file;
}

OpenFile* PCB::GetFile(int id) {
    if (!fileBitmap->Test(id)) return NULL;
    return filetable[id];
}

int PCB::GetID() { return pid; }
int PCB::GetNumWait() { return numwait; }
Thread* PCB::getThread() { return thread; }