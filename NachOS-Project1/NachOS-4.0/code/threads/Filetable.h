// FileTable.h
// File table

// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FileTable_H
#define FileTable_H

#include "bitmap.h"
#define MAX_FILE 10

typedef int OpenFileID;

class Filetable {
    private:
        Bitmap* bm;
        FILE* file[MAX_FILE];
    public:
        Filetable();
        int CreateFile(char* name);
        OpenFileID OpenFile(char* name, int type);
        int CloseFile(OpenFileID id);
        int ReadFile(char* buffer, int charcount, OpenFileID id);
        int WriteFile(char* buffer, int charcount, OpenFileID id);
        ~Filetable(); 
        // Filetable(){
        //     bm = new Bitmap(MAX_FILE);
        //     // ouput to screen console
        //     bm->Mark(0);
        //     // input from keyboard
        //     bm->Mark(1);

        //     for(int i=0; i<MAX_FILE; ++i){
        //         file[i] = NULL;
        //     }
        // }

        // int CreateFile(char* filename){
        //     char cwd[PATH_MAX];
        //     getcwd(cwd, sizeof(cwd));
        //     char sbuf[1024];
        //     sprintf (sbuf, "%s/%s", cwd, filename);
        //     int result = mknod(sbuf, S_IFREG|0666, 0);
        //     return result != 0 ? -1 : 0;
        // }

        // OpenFileID OpenFile(char* name, int type){
        //     if (type != 0 && type != 1){
        //         return -1;
        //     }

        //     int id = bm->FindAndSet(); // find free slot
        //     if (id == -1){  // not enough memory
        //         return -1;
        //     }    

        //     char cwd[PATH_MAX];
        //     getcwd(cwd, sizeof(cwd));
        //     char sbuf[1024];
        //     sprintf (sbuf, "%s/%s", cwd, name);

        //     const char* mode = type == 0 ? "r+b" : "rb";
        //     file[id] = fopen(sbuf, mode);

        //     // if file name not found or some other error occurred
        //     if (file[id]) return id;
        //     return -1;
        // }

        // int ReadFile(char* buffer, int charcount, OpenFileID id){
        //     if (!bm->Test(id)){
        //         // file is not opened yet
        //         return -1;
        //     }

        //     int result = fread(buffer, 1, charcount, file[id]);
        //     if (result == 0){
        //         return -2;
        //     }
        //     return result;
        // }

        // int WriteFile(char* buffer, int charcount, OpenFileID id){
        //     if (!bm->Test(id)){
        //         // file is not opened yet
        //         return -1;
        //     }
        //     int result = fwrite(buffer, 1, charcount, file[id]);
        //     return result;
        // }

        // int CloseFile(OpenFileID id){
        //     if (!bm->Test(id)){
        //         // file not opened yet
        //         return -1;
        //     }

        //     bm->Clear(id);
        //     file[id] = NULL;
        //     return 0;
        // }
        
};

#endif