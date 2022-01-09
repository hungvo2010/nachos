// process control block

#ifndef PCB_H
#define PCB_H
#include "thread.h"
#include "synch.h"
#define MAX_FILE 10
class Semaphore;
class PCB{
private:
    Semaphore *joinsem; //semaphore cho quá trình join
    Semaphore *exitsem; //semaphore cho quá trình exit
    Semaphore *mutex;
    int exitcode;
    Thread *thread;
    int pid;
    int numwait; // số tiến trình đã join
    OpenFile* filetable[MAX_FILE];
    Bitmap* fileBitmap;
    AddrSpace *space;
public:
    int parentID; //ID của tiến trình cha
    PCB(int id);
    ~PCB();
    int Exec(char *filename, int pid); //Nạp chương trình có tên lưu trong biến filename và pid là ProcessId


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
