// PCB.h
// Process control block

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef PCB_H
#define PCB_H

#include "synch.h"
#include "thread.h"
#include "FileTable.h"
#include "main.h"
typedef int OpenFileID;

extern void StartProcess_2(int id);

class PCB {
    private:
        Semaphore* joinsem = NULL;             // semaphore for join process
        Semaphore* exitsem = NULL;             // semaphore for exit process
        Semaphore* multex = NULL;              // semaphore for unique access
        int exitcode;                   //    
        int numwait;                    // number of process joined
        Thread* thread;
        Filetable* filetable;
    public:
        int parentID;        
        PCB(){
            this->parentID = kernel->currentThread->processID;
            this->numwait = this->exitcode = 0;
            this->thread = NULL;

            this->joinsem = new Semaphore("joinsem",0);
            this->exitsem = new Semaphore("exitsem",0);
            this->multex = new Semaphore("multex",1);

            this->filetable = new Filetable();
        }
        PCB(int id){
            if (id == 0){
                this->parentID = -1;
            }
            else {
                this->parentID = kernel->currentThread->processID;

                this->numwait = this->exitcode = 0;
                this->thread = NULL;

                this->joinsem = new Semaphore("joinsem",0);
                this->exitsem = new Semaphore("exitsem",0);
                this->multex = new Semaphore("multex",1);
            }
        }
        ~PCB(){
            if(joinsem != NULL)
                delete this->joinsem;
            if(exitsem != NULL)
                delete this->exitsem;
            if(multex != NULL)
                delete this->multex;
            if(this->thread != NULL)
            {		
                this->thread->FreeSpace();
                this->thread->Finish();		
            }
        }
        int Exec(char* filename, int id)
        {  
            // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
            multex->P();

            // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
            this->thread = new Thread(filename); // (./threads/thread.h)

            if(this->thread == NULL){
                printf("\nExec: Not enough memory!\n");
                    multex->V(); // Nha CPU de nhuong CPU cho tien trinh khac
                return -1; // Tra ve -1 neu that bai
            }

            //  Đặt processID của thread này là id.
            this->thread->processID = id;
            // Đặt parrentID của thread này là processID của thread gọi thực thi Exec
            this->parentID = kernel->currentThread->processID;
            // Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
            this->thread->Fork(StartProcess_2, id);

                multex->V();
            // Trả về id.
            return id;

        }

        void JoinWait()
        {
            //Gọi joinsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ JoinRelease để thực hiện tiếp.
            joinsem->P();
        }

        void JoinRelease()
        { 
            // Gọi joinsem->V() để giải phóng tiến trình gọi JoinWait().
            joinsem->V();
        }

        void ExitWait()
        { 
            // Gọi exitsem-->V() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ ExitReleaseđể thực hiện tiếp.
            exitsem->P();
        }

        void ExitRelease() 
        {
            // Gọi exitsem-->V() để giải phóng tiến trình đang chờ.
            exitsem->V();
        }

        int GetID(){
            return this->thread->processID;
        }

        char* GetFileName(){
            return this->thread->getName();
        }

        void SetExitCode(int ec){
            this->exitcode = ec;
        }

        int GetExitCode(){
            return this->exitcode;
        }

        void SetFileName(char* fn){
            this->thread->setName(fn);
        }

        int GetNumWait(){
            return this->numwait;
        }

        void IncNumWait(){
            this->multex->P();
            this->numwait++;
            this->multex->V();
        }

        void DecNumWait(){
            this->multex->P();
            this->numwait--;
            this->multex->V();
        }

        int CreateFile(char* filename){
            return this->filetable->CreateFile(filename);
        }

        OpenFileID OpenFile(char* filename, int type){
            return this->filetable->OpenFile(filename, type);
        }

        int ReadFile(char* buffer, int charcount, OpenFileID id){
            return this->filetable->ReadFile(buffer, charcount, id);
        }

        int WriteFile(char* buffer, int charcount, OpenFileID id){
            return this->filetable->WriteFile(buffer, charcount, id);
        }

        int CloseFile(OpenFileID id){
            return this->filetable->CloseFile(id);
        }

        // PCB();
        // PCB(int id);
        // ~PCB();

        // int Exec(char *filename, int pid);
        // int GetID();
        // int GetNumWait(); 
        // void IncNumWait();
        // void DecNumWait(); 
        // void JoinWait();
        // void ExitWait();
        // void JoinRelease();
        // void ExitRelease();
        // void SetExitCode(int ec);
        // int GetExitCode();
        // void SetFileName(char* fn);
        // char* GetFileName();

        // int CreateFile(char* filename);
        // OpenFileID OpenFile(char* filename, int type);
        // int ReadFile(char* buffer, int charcount, OpenFileID id);
        // int WriteFile(char* buffer, int charcount, OpenFileID id);
        // int CloseFile(OpenFileID id);
};

#endif // PCB_H