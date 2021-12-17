#include "PCB.h"
#include "progtest.cc"
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
 	this->thread->Fork(StartProcess_2, id);

    	multex->V();
	// Trả về id.
	return id;

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
	this->numwait++;
}

void PCB::DecNumWait(){
	this->numwait--;
}

void PCB::JoinWait(){

}
void PCB::ExitWait(){

}
void PCB::JoinRelease(){

}
void PCB::ExitRelease(){
	
}