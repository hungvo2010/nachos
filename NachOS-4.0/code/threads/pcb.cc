#include "pcb.h"

PCB::PCB(int id)
{
	pid = id;
	joinsem = new Semaphore("Join Semaphore", 0);
	exitsem = new Semaphore("Exit Semaphore", 0);
	mutex = new Semaphore("Condition Semaphore", 1);

	FileDescriptorTable[0] = kernel->fileSystem->OpenFileTable[0];
	FileDescriptorTable[1] = kernel->fileSystem->OpenFileTable[1];

	fdBitmap = new Bitmap(MAX_FILE_1_PROCESS);
	fdBitmap->Mark(0);
	fdBitmap->Mark(1);

}
PCB::~PCB()
{
	delete joinsem;
	delete exitsem;
	delete mutex;
	delete fdBitmap;

	if (thread) delete thread;
}

int PCB::GetID() { return pid; }
int PCB::GetNumWait(){return numwait;}
void PCB::JoinWait()
{
	joinsem->P();
}
void PCB::ExitWait()
{
	exitsem->P();
}
void PCB::JoinRelease()
{
	joinsem->V();
}
void PCB::ExitRelease()
{
	exitsem->V();
}
void PCB::IncNumWait()
{
	mutex->P();
	numwait++;
	mutex->V();
}
void PCB::DecNumWait()
{
	mutex->P();
	numwait--;
	mutex->V();
}
void PCB::SetExitCode(int ec) { exitcode = ec; }
int PCB::GetExitCode() { return exitcode; }
Thread* PCB::getThread() { return thread; }

void StartProcess_2(int id);
int PCB::Exec(char *filename, int id)
{
	// Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	mutex->P();

	// Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.
	this->thread = new Thread(filename); // (./threads/thread.h)

	if (this->thread == NULL)
	{
		printf("\nPCB::Exec: Not enough memory!\n");
		mutex->V(); // Nha CPU de nhuong CPU cho tien trinh khac
		return -1;	// Tra ve -1 neu that bai
	}

	//  Đặt processID của thread này là id.
	this->thread->processID = id;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	this->parentID = kernel->currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
	this->thread->Fork((VoidFunctionPtr)StartProcess_2, (void *)id);

	mutex->V();
	// Trả về id.
	return id;
}

void StartProcess_2(int id)
{
	// Lay fileName cua process id nay
	char *fileName = kernel->pTab->GetFileName(id);

	AddrSpace *space;
	space = new AddrSpace();
	if (space == NULL) return;
	space->Load(fileName);

	if (space == NULL)
	{
		printf("\nPCB::Exec: Can't create AddSpace.");
		return;
	}
	kernel->pTab->addrspace[id] = space;
	space->Execute();
}

bool PCB::isFileTableFull()
{
	return fdBitmap->NumClear() == 0;
}

int PCB::Open(OpenFile* new_file)
{
	if (isFileTableFull()) return -1;
	int free_slot = fdBitmap->FindAndSet();
	FileDescriptorTable[free_slot] = new_file;
	return free_slot;
}

OpenFile*  PCB::Close(int file_id)
{
	if (!fdBitmap->Test(file_id)) return NULL;
	OpenFile* tmp = FileDescriptorTable[file_id];
	fdBitmap->Clear(file_id);
	FileDescriptorTable[file_id] = NULL;
	return tmp;
}

OpenFile* PCB::GetFile(int file_id)
{
	if (!fdBitmap->Test(file_id)) return NULL;
	return FileDescriptorTable[file_id];
}