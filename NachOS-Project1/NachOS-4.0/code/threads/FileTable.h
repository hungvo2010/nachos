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
};

#endif