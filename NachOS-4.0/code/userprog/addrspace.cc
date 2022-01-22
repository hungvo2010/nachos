// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -n -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you are using the "stub" file system, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "addrspace.h"
#include "machine.h"
#include "noff.h"
#include "synch.h"

static void 
SwapHeader (NoffHeader *noffH)
{
    noffH->noffMagic = WordToHost(noffH->noffMagic);
    noffH->code.size = WordToHost(noffH->code.size);
    noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
#ifdef RDATA
    noffH->readonlyData.size = WordToHost(noffH->readonlyData.size);
    noffH->readonlyData.virtualAddr = 
           WordToHost(noffH->readonlyData.virtualAddr);
    noffH->readonlyData.inFileAddr = 
           WordToHost(noffH->readonlyData.inFileAddr);
#endif 
    noffH->initData.size = WordToHost(noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost(noffH->uninitData.size);
    noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);

#ifdef RDATA
    DEBUG(dbgAddr, "code = " << noffH->code.size <<  
                   " readonly = " << noffH->readonlyData.size <<
                   " init = " << noffH->initData.size <<
                   " uninit = " << noffH->uninitData.size << "\n");
#endif
}

AddrSpace::AddrSpace()
{

}

AddrSpace::~AddrSpace()
{
    for (int i = 0; i < numPages; ++i)
        kernel->physicalMemory->Clear(pageTable[i].physicalPage);
   delete pageTable;
}

bool 
AddrSpace::Load(char *fileName) 
{
    NoffHeader noffH;
    int numCodePage, numDataPage;
    int lastCodePageSize, lastDataPageSize, firstDataPageSize, tmpDataSize;
    OpenFile *executable = kernel->fileSystem->Open(fileName);
    unsigned int size;

    if (executable == NULL) {
        cerr << "Unable to open file " << fileName << "\n";
        return FALSE;
    }

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

    kernel->addrLock->Acquire();

#ifdef RDATA
    size = noffH.code.size + noffH.readonlyData.size + noffH.initData.size +
           noffH.uninitData.size + UserStackSize;	
#else
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;
#endif
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    if (numPages > kernel->physicalMemory->NumClear())
    {
        numPages = 0;
        delete executable;
        kernel->addrLock->Release();
        return false;
    }
    pageTable = new TranslationEntry[numPages];
    for (int i = 0; i < numPages; i++) 
    {
        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = kernel->physicalMemory->FindAndSet();
        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  
        bzero(&(kernel->machine->mainMemory[pageTable[i].physicalPage*PageSize]), PageSize);
    }
    
    kernel->addrLock->Release(); 
    numCodePage = divRoundUp(noffH.code.size, PageSize); 
    lastCodePageSize = noffH.code.size - (numCodePage-1)*PageSize;
    tmpDataSize = noffH.initData.size - (PageSize - lastCodePageSize); 

    if (tmpDataSize < 0){
        numDataPage = 0;
        firstDataPageSize = noffH.initData.size;
    } 
    else{
        numDataPage = divRoundUp(tmpDataSize, PageSize);
        lastDataPageSize = tmpDataSize - (numDataPage-1)*PageSize;
        firstDataPageSize = PageSize - lastCodePageSize;
    } 

    
    DEBUG(dbgAddr, "Initializing address space: " << numPages << ", " << size);

    int i = 0;
    DEBUG(dbgAddr, "Initializing code segment.");
	DEBUG(dbgAddr, noffH.code.virtualAddr << ", " << noffH.code.size);
    for (i = 0; i < numCodePage; i++) 
    {
        executable->ReadAt(&(kernel->machine->mainMemory[noffH.code.virtualAddr]) + 
        pageTable[i].physicalPage*PageSize, i<(numCodePage-1) ? PageSize : lastCodePageSize,
        noffH.code.inFileAddr + i*PageSize);
    }
    if (lastCodePageSize < PageSize)
    {
        if (firstDataPageSize > 0) 
        executable->ReadAt(&(kernel->machine->mainMemory[noffH.code.virtualAddr])+(pageTable[i-1].physicalPage*PageSize + 
            lastCodePageSize), firstDataPageSize, noffH.initData.inFileAddr);
    } 

    DEBUG(dbgAddr, "Initializing data segment.");
	DEBUG(dbgAddr, noffH.initData.virtualAddr << ", " << noffH.initData.size);
    int j = 0;
    for (j = 0; j< numDataPage; j++) 
    {
        executable->ReadAt(&(kernel->machine->mainMemory[noffH.code.virtualAddr])+pageTable[i].physicalPage*PageSize,
        j < (numDataPage-1) ? PageSize : lastDataPageSize,
        noffH.initData.inFileAddr + j*PageSize + firstDataPageSize);
        i++;
    }

#ifdef RDATA
    if (noffH.readonlyData.size > 0) {
        DEBUG(dbgAddr, "Initializing read only data segment.");
	DEBUG(dbgAddr, noffH.readonlyData.virtualAddr << ", " << noffH.readonlyData.size);
        executable->ReadAt(
		&(kernel->machine->mainMemory[noffH.readonlyData.virtualAddr]),
			noffH.readonlyData.size, noffH.readonlyData.inFileAddr);
    }
#endif

    delete executable;
    return TRUE;
}

void 
AddrSpace::Execute() 
{

    kernel->currentThread->space = this;

    this->InitRegisters();
    this->RestoreState();

    kernel->machine->Run();

    ASSERTNOTREACHED();
}

void
AddrSpace::InitRegisters()
{
    Machine *machine = kernel->machine;
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    machine->WriteRegister(PCReg, 0);
    machine->WriteRegister(NextPCReg, 4);
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG(dbgAddr, "Initializing stack pointer: " << numPages * PageSize - 16);
}

void AddrSpace::SaveState() 
{

}

void AddrSpace::RestoreState() 
{
    kernel->machine->pageTable = pageTable;
    kernel->machine->pageTableSize = numPages;
}

ExceptionType
AddrSpace::Translate(unsigned int vaddr, unsigned int *paddr, int isReadWrite)
{
    TranslationEntry *pte;
    int               pfn;
    unsigned int      vpn    = vaddr / PageSize;
    unsigned int      offset = vaddr % PageSize;

    if(vpn >= numPages) {
        return AddressErrorException;
    }

    pte = &pageTable[vpn];

    if(isReadWrite && pte->readOnly) {
        return ReadOnlyException;
    }

    pfn = pte->physicalPage;

    if (pfn >= NumPhysPages) {
        DEBUG(dbgAddr, "Illegal physical page " << pfn);
        return BusErrorException;   
    }

    pte->use = TRUE;

    if(isReadWrite)
        pte->dirty = TRUE;

    *paddr = pfn*PageSize + offset;

    ASSERT((*paddr < MemorySize));

    return NoException;
}




