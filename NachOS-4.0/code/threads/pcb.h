// process control block

#ifndef PCB_H
#define PCB_H
#include "synch.h"
#include "thread.h"
#define MAX_FILE 10
class Semaphore;
class PCB {
   private:
    Semaphore* joinsem;  //semaphore cho join
    Semaphore* exitsem;  //semaphore cho exit
    Semaphore* multex;
    int exitcode;
    Thread* thread;
    int pid;
    int numwait;  // so luong tien trinh da join
    OpenFile* filetable[MAX_FILE];
    Bitmap* fileBitmap;
    AddrSpace* space;

   public:
    int parentID;  // ID cua process cha
    PCB(int id);
    ~PCB();
    int Exec(char* filename, int pid);

    int GetID();
    int GetNumWait();
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    void SetExitCode(int ec);
    int GetExitCode();
    Thread* getThread();
    bool isFileTableFull();
    int Open(OpenFile* new_file);
    OpenFile* Close(int file_id);
    OpenFile* GetFile(int file_id);
};
#endif
