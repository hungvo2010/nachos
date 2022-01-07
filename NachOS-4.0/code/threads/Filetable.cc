#include "Filetable.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

Filetable::Filetable(){
    this->bm = new Bitmap(MAX_FILE);
    // ouput to screen console
    this->bm->Mark(0);
    // input from keyboard
    this->bm->Mark(1);
    this->mode = new int[MAX_FILE];
    memset(this->mode, -1, MAX_FILE);
    for(int i=2; i<2; ++i){
        file[i] = NULL;
    }
}

int Filetable::CreateFile(char* filename){
    char cwd[PATH_MAX];
    // get current directory
    getcwd(cwd, sizeof(cwd));
    char sbuf[1024];
    // absolute path to new file
    sprintf (sbuf, "%s/%s", cwd, filename);
    int result = mknod(sbuf, S_IFREG|0666, 0);
    return result != 0 ? -1 : 0;
}

OpenFileID Filetable::OpenFile(char* name, int type){
    int id = this->bm->FindAndSet(); // find free slot
    
    if (id == -1){  // not enough memory
        return -1;
    }    

    char cwd[PATH_MAX];
    // get current directory
    getcwd(cwd, sizeof(cwd));
    char sbuf[1024];
    // absolute path to new file
    sprintf (sbuf, "%s/%s", cwd, name);

    const char* filemode = type == 0 ? "rb+" : "rb";
    FILE* fi = fopen(sbuf, filemode);
    if (!fi) return -1;
    file[id] = fi;
    mode[id] = type;
    // if file name not found or some other error occurred
    return id;
}

int Filetable::ReadFile(char* buffer, int charcount, OpenFileID id){
    if (!bm->Test(id)){
        // file is not opened yet
        return -1;
    }

    // read "charcount" character from file
    int result = fread(buffer, 1, min(charcount, strlen(buffer)), file[id]);
    // end of file
    if (result == 0){
        return -2;
    }
    return result;
}

int Filetable::WriteFile(char* buffer, int charcount, OpenFileID id){
    if (!bm->Test(id)){
        // file is not opened yet
        return -1;
    }

    // write to read only file mode
    if (mode[id] == 1){
        return -1;
    }

    // write "charcount" character to file
    int result = fwrite(buffer, 1, min(strlen(buffer), charcount), file[id]);
    return result;
}

int Filetable::CloseFile(OpenFileID id){
    if (!bm->Test(id)){
        // file not opened yet
        return -1;
    }

    bm->Clear(id);
    fclose(file[id]);
    mode[id] = -1;
    file[id] = NULL;
    return 0;
}

Filetable::~Filetable(){
    if (bm) delete bm;
    if (mode) delete[] mode;
    for(int i = 2; i < MAX_FILE; ++i){
        if (file[i]){
            fclose(file[i]);
        }
    }
}