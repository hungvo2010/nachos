// STable.h
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"
#define MAX_SEMAPHORE 10


// Lop Sem dung de quan ly semaphore.
class Sem
{
private:
	char name[50];		// Ten cua semaphore
	Semaphore* sem;		// Tao semaphore de quan ly
public:
	// Khoi tao doi tuong Sem. Gan gia tri ban dau la null
	// Nho khoi tao sem su dung
	Sem(char* na, int i)
	{
		strcpy(this->name, na);
		sem = new Semaphore(this->name, i);
	}

	~Sem()
	{
		if(sem)
			delete sem;
	}

	void wait()
	{
		sem->P();	// Down(sem)
	}

	void signal()
	{
		sem->V();	// Up(sem)
	}
	
	char* GetName()
	{
		return this->name;
	}
};

class STable
{
private:
	Bitmap* bm;	// quản lý slot trống
	Sem* semTab[MAX_SEMAPHORE];	// quản lý tối đa 10 đối tượng Sem
public:
	STable(){
		this->bm = new Bitmap(MAX_SEMAPHORE);

		for(int i=0; i<MAX_SEMAPHORE; ++i){
			this->semTab[i] = NULL;
		}
	}

	~STable(){
		if(this->bm){
			delete this->bm;
			this->bm = NULL;
		}
	}

	int Create(char* name, int init){
		for(int i = 0; i < MAX_SEMAPHORE; ++i){
			if (bm->Test(i)){
				// if two string is equal
				if (strcmp(name, semTab[i]->GetName()) == 0){
					return -1;
				}
			}
		}
		int id = this->FindFreeSlot();
		if (id < 0){
			return -1;
		}
		this->semTab[id] = new Sem(name, init);
		return 0;
	}

	int Wait(char* name){
		for(int i = 0; i < MAX_SEMAPHORE; ++i){
			if (bm->Test(i)){
				// if two string is equal
				if (strcmp(name, semTab[i]->GetName()) == 0){
					semTab[i]->wait();
					return 0;
				}
			}
		}
		return -1;
	}

	int Signal(char* name){
		for(int i = 0; i < MAX_SEMAPHORE; ++i){
			if (bm->Test(i)){
				// if two string is equal
				if (strcmp(name, semTab[i]->GetName()) == 0){
					semTab[i]->signal();
					return 0;
				}
			}
		}
		return -1;
	}

	int FindFreeSlot(){
		return bm->FindAndSet();
	}

	// //khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null
	// // nhớ khởi tạo bm để sử dụng
	// STable();		

	// ~STable();	// hủy các đối tượng đã tạo
	// // Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi.
	// int Create(char *name, int init);

	// // Nếu tồn tại Semaphore “name” thì gọi this->P()để thực thi. Ngược lại, báo lỗi.
	// int Wait(char *name);

	// // Nếu tồn tại Semaphore “name” thì gọi this->V()để thực thi. Ngược lại, báo lỗi.
	// int Signal(char *name);
	
	// // Tìm slot trống.
	// int FindFreeSlot();

	
};
#endif // STABLE_H