//stable.h
#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"
#include "sem.h"
#define MAX_SEMAPHORE 10
class Sem;
class STable
{
private:
    Bitmap* bm;
    Sem* semTab[MAX_SEMAPHORE];
    int findByName(char* name);
public:
    STable();
    ~STable();
    int Create(char *name, int value);
    int Wait(char *name);
    int Signal(char *name);
};
#endif
