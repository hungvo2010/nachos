#include "STable.h"

STable::STable(){
    this->bm = new Bitmap(MAX_SEMAPHORE);

    for(int i=0; i<MAX_SEMAPHORE; ++i){
        this->semTab[i] = NULL;
    }
}

STable::~STable(){
    if(this->bm){
        delete this->bm;
        this->bm = NULL;
    }
}