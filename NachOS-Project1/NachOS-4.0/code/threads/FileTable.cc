#include "FileTable.h"

#include <unistd.h>
#include <limits.h>
#include <string.h>

FileTable::FileTable(){
    // ouput to screen console
    this->file[0] = 0;
    // input from keyboard
    this->file[1] = 1;

    for(int i=2; i<MAX_FILE; ++i){
        // not opened yet
        this->file[i] = -1;
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
