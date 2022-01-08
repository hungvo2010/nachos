#include "syscall.h"
#include "copyright.h"

void main()
{
	int file_success; // Bien co dung de kiem tra thanh cong
	SpaceId  fileid_voinuoc, fileid_result;	// Bien id cho file
	char c_readFile;	// Bien ki tu luu ki tu doc tu file
	int v1, v2;		// Voi 1, voi 2
	int dungtich;			// Dung tich nuoc cua sinh vien
	int flag_done_result;	// Bien co luu dau hieu doc xong file result

	v1 = v2 = 0;
	// Xu ly voi nuoc
	// WHILE(11111111111111111111111111111111111111)
	while(1)
	{
		Wait("m_vn");

		// Mo file result.txt de ghi voi nao su dung
		fileid_result = Open("result.txt", 0);
		if(fileid_result == -1)
		{		
			Signal("sinhvien");
			return;
		}
		// WHILE(2222222222222222222222222222222222222222)
		while(1)
		{
			Wait("voinuoc");
			c_readFile = 0;			
			// Mo file voi nuoc .txt de doc dung tich
			fileid_voinuoc = Open("voinuoc.txt", 1);
			if(fileid_voinuoc == -1)
			{
				Close(fileid_result);
				Signal("sinhvien");			
				return;
			}
		
			dungtich = 0;
			flag_done_result = 0;
			// WHILE(3333333333333333333333333333333333333333333333)
			while(1)
			{			
				if(Read(&c_readFile, 1, fileid_voinuoc)  == -2)
				{	
					Close(fileid_voinuoc);			
					break;
				}
				if(c_readFile != '*')
				{
					dungtich = dungtich* 10 + (c_readFile - 48);
				}
				else
				{
					flag_done_result = 1;				
					Close(fileid_voinuoc);
					break;			
				}
			}
			// WHILE(3333333333333333333333333333333333333333333333)
			if(dungtich!= 0)
			{
				// Dung voi 1
				if(v1 <= v2)
				{
					v1 += dungtich;
					Write("1", 1, fileid_result);
				}
				else	// Dung voi 2
				{					
					v2 += dungtich;
					Write("2", 1, fileid_result);
				}
			}
		
			if(flag_done_result == 1)
			{
				v1 = v2 = 0;
				Close(fileid_result);				
				Signal("sinhvien");
				break;				
			}

			Signal("sinhvien");
		}
		// WHILE(2222222222222222222222222222222222222222)	
	}
	// WHILE(11111111111111111111111111111111111111)			
}
