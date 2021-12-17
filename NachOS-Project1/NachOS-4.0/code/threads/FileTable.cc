#include "FileTable.h"

#include <unistd.h>
#include <limits.h>
#include <string.h>

FileTable::FileTable(){
    this->file[0] = 0;
    this->file[1] = 1;
    for(int i=2; i<MAX_FILE; ++i){
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
