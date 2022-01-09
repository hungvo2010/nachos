#include "syscall.h"

#define L_INTRO 500
char INTRO[L_INTRO] = "Thanh vien:\n\
	19120033\tPhan Loc Son\n\
	19120732\tNguyen Xuan Vy\n\
	19120734\tLe Minh Tu\n\n";

#define L_ASCII 500
char ASCII[L_ASCII] = "ascii.c:\n\
	Chuong trinh se dung vong for de in ra cac so \n\
	co ma ascii tu 50 den 127\n\n";

#define L_SORT 500
char SORT[L_SORT] = "sort.c:\n\
	Chuong trinh se cho nguoi dung nhap vao n (size cua mang), sau do nhap tung phan tu cua mang vao,\n\
	cuoi cung, chuong  trinh se in ra mang da sort\n\n";
int 
main()
{
	PrintString(INTRO, L_INTRO);
	PrintString(ASCII, L_ASCII);
	PrintString(SORT, L_SORT);
	Halt();
}
