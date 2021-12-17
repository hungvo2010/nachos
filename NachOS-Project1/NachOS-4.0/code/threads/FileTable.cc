#include "FileTable.h"

#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

FileTable::FileTable(){
    bm = new Bitmap(MAX_FILE);
    // ouput to screen console
    bm->Mark(0);
    // input from keyboard
    bm->Mark(1);

    for(int i=0; i<MAX_FILE; ++i){
        file[i] = NULL;
    }
}

int FileTable::CreateFile(char* filename){
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    char sbuf[1024];
    sprintf (sbuf, "%s/%s", cwd, filename);
    int result = mknod(sbuf, S_IFREG|0666, 0);
    return result != 0 ? -1 : 0;
}

OpenFileID Filetable::OpenFile(char* name, int type){
    if (type != 0 && type != 1){
        return -1;
    }

    int id = bm->FindAndSet();
    if (id == -1){
        return -1;
    }    

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    char sbuf[1024];
    sprintf (sbuf, "%s/%s", cwd, filename);

    char* mode = type == 0 ? 'w+b' : 'rb';
    file[id] = fopen(sbuf, mode);
    return id;
}

int FileTable::ReadFile(char* buffer, int charcount, OpenFileID id){
    if (!bm->Test(id)){
        // file is not opened yet
        return -1;
    }
    char* temp = new char[charcount];
    int result = fread(temp, charcount, 1, file[id]);
    strcpy(buffer, temp);
    return result;

}

int FileTable::WriteFile(char* buffer, int charcount, OpenFileID id){
    if (!bm->Test(id)){
        // file is not opened yet
        return -1;
    }
    int result = fwrite(buffer, charcount, 1, file[id]);
    return result;
}

int FileTable::CloseFile(OpenFileID id){
    if (!bm->Test(id)){
        // file not opened yet
        return -1;
    }

    bm->Clear(id);
    file[id] = NULL;
    return 0;
}
