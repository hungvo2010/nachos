#include "pcb.h"

PCB::PCB(int id){
	pid = id;
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);

	this->filetable[0] = kernel->fileSystem->filetable[0];
	this->filetable[1] = kernel->fileSystem->filetable[1];

	this->fileBitmap = new Bitmap(MAX_FILE);
	this->fileBitmap->Mark(0);
	this->fileBitmap->Mark(1);

}
PCB::~PCB()
{
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(fileBitmap) delete fileBitmap;

	if (thread) delete thread;
}

void PCB::JoinWait()
{
	//Gọi joinsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ JoinRelease để thực hiện tiếp.
    joinsem->P();
}

void PCB::JoinRelease()
{ 
	// Gọi joinsem->V() để giải phóng tiến trình gọi JoinWait().
    joinsem->V();
}

void PCB::ExitWait()
{ 
	// Gọi exitsem-->V() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ ExitReleaseđể thực hiện tiếp.
    exitsem->P();
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
	return fileBitmap->NumClear() == 0;
}

int PCB::Open(OpenFile* new_file)
{
	if (isFileTableFull()) return -1;
	int id = fileBitmap->FindAndSet();
	filetable[id] = new_file;
	return id;
}

OpenFile*  PCB::Close(int id)
{
	if (!fileBitmap->Test(id)) return NULL;
	OpenFile* file = filetable[id];
	fileBitmap->Clear(id);
	filetable[id] = NULL;
	return file;
}

OpenFile* PCB::GetFile(int id)
{
	if (!fileBitmap->Test(id)) return NULL;
	return filetable[id];
}

int PCB::GetID() { return pid; }
int PCB::GetNumWait(){return numwait;}
Thread* PCB::getThread() { return thread; }