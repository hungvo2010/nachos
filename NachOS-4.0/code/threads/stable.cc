#include "stable.h"

STable::STable()
{
    bm = new Bitmap(MAX_SEMAPHORE);
}

STable::~STable()
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
            delete semTab[i];
    }
    delete bm;
}

int STable::Create(char *name, int value)
{
    int free = bm->FindAndSet();
    if (free == -1)
        return -1;
    semTab[free] = new Sem(name, value);
    return free;
}
int STable::Wait(char *name)
{
    int found = findByName(name);
    if (found == -1)
        return -1;
    else
        semTab[found]->wait();
}
int STable::Signal(char *name)
{
    int found = findByName(name);
    if (found == -1)
        return -1;
    else
    {
        semTab[found]->signal();
        return 0;
    }
}
int STable::findByName(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; ++i)
    {
        if (bm->Test(i))
        {
            if (!strcmp(name, semTab[i]->GetName()))
                return i;
        }
    }
    return -1;
}