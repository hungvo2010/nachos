#include "PCB.h"
#include "main.h"
void StartProcess_2(int id)
{
	char* fileName = pTab->GetFileName(id);

	AddrSpace *space;
	space = new AddrSpace(fileName);

	if(space == NULL)
	{
		printf("\nPCB::Exec : Can't create AddSpace.");
		return;
	}

	kernel->currentThread->space = space;

	space->InitRegisters();		
	space->RestoreState();		

	kernel->machine->Run();		
	ASSERT(FALSE);		
}

PCB::PCB(){
	this->parentID = kernel->currentThread->processID;
	this->numwait = this->exitcode = 0;
	this->thread = NULL;

	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);

	this->filetable = new Filetable();
}
PCB::PCB(int id){
	if (id == 0){
		this->parentID = -1;
	}
	else {
		this->parentID = kernel->currentThread->processID;
	}
	this->numwait = this->exitcode = 0;
	this->thread = NULL;

	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);

	this->filetable = new Filetable();
}
PCB::~PCB(){
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(this->thread != NULL)
	{		
		this->thread->FreeSpace();
		this->thread->Finish();		
	}
	if(this->filetable != NULL){
		delete this->filetable;
	}
}
int PCB::Exec(char* filename, int id)
{  
    // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	multex->P();

    // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
	this->thread = new Thread(filename); // (./threads/thread.h)

	if(this->thread == NULL){
		printf("\nPCB::Exec: Not enough memory!\n");
        	multex->V(); // Nha CPU de nhuong CPU cho tien trinh khac
		return -1; // Tra ve -1 neu that bai
	}

	//  Đặt processID của thread này là id.
	this->thread->processID = id;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	this->parentID = kernel->currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
 	this->thread->Fork((VoidFunctionPtr)StartProcess_2, (void*)id);

    	multex->V();
	// Trả về id.
	return id;

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

void PCB::ExitRelease() 
{
	// Gọi exitsem-->V() để giải phóng tiến trình đang chờ.
    exitsem->V();
}

int PCB::GetID(){
	return this->thread->processID;
}

char* PCB::GetFileName(){
	return this->thread->getName();
}

void PCB::SetExitCode(int ec){
	this->exitcode = ec;
}

int PCB::GetExitCode(){
	return this->exitcode;
}

void PCB::SetFileName(char* fn){
	this->thread->setName(fn);
}

int PCB::GetNumWait(){
	return this->numwait;
}

void PCB::IncNumWait(){
	this->multex->P();
	this->numwait++;
	this->multex->V();
}

void PCB::DecNumWait(){
	this->multex->P();
	this->numwait--;
	this->multex->V();
}

int PCB::CreateFile(char* filename){
    return this->filetable->CreateFile(filename);
}

OpenFileID PCB::OpenFile(char* filename, int type){
    return this->filetable->OpenFile(filename, type);
}

int PCB::ReadFile(char* buffer, int charcount, OpenFileID id){
	return this->filetable->ReadFile(buffer, charcount, id);
}

int PCB::WriteFile(char* buffer, int charcount, OpenFileID id){
	return this->filetable->WriteFile(buffer, charcount, id);
}

int PCB::CloseFile(OpenFileID id){
	return this->filetable->CloseFile(id);
}