#include "syscall.h"
#include "copyright.h"

void main()
{
	// Khai bao
	int file_success; // Bien co dung de kiem tra thanh cong
	SpaceId fileid_sinhvien, fileid_voinuoc;	// Bien id cho file
	char c_readFile;	// Bien ki tu luu ki tu doc tu file
	int flag_voinuoc;		// Bien co de nhay den tien trinh voinuoc
	int flag_main;		// Bien co de nhay den tien trinh main
	int lengthFile;		// Luu do dai file
	int i_File;		// Luu con tro file
	//-----------------------------------------------------------
	Signal("m_vn");	

	while(1)
	{
		lengthFile = 0;

		Wait("sinhvien");
		
		// Tao file result.txt de ghi voi nao su dung
		file_success = CreateFile("result.txt");
		if(file_success == -1)
		{
			Signal("main"); // tro ve tien trinh chinh
			return;
		}

		// Mo file sinhvien.txt len de doc
		fileid_sinhvien = Open("sinhvien.txt", 1);
		if(fileid_sinhvien == -1)
		{
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		
		lengthFile = Seek(-1, fileid_sinhvien);
		Seek(0, fileid_sinhvien);
		i_File = 0;
	
		// Tao file voinuoc.txt
		file_success = CreateFile("voinuoc.txt");
		if(file_success == -1)
		{
			Close(fileid_sinhvien);
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		

		// Mo file voinuoc.txt de ghi tung dung tich nuoc cua sinhvien
		fileid_voinuoc = Open("voinuoc.txt", 0);
		if(fileid_voinuoc == -1)
		{
			Close(fileid_sinhvien);
			Signal("main"); // tro ve tien trinh chinh
			return;
		}
		
		// Ghi dung tich vao file voinuoc.txt tu file sinhvien.txt
		while(i_File < lengthFile)
		{
			flag_voinuoc = 0;
			Read(&c_readFile, 1, fileid_sinhvien);
			if(c_readFile != ' ')
			{
				Write(&c_readFile, 1, fileid_voinuoc);
			}
			else
			{
				flag_voinuoc = 1;
			}
			if(i_File == lengthFile - 1)
			{
				Write("*", 1, fileid_voinuoc);
				flag_voinuoc = 1;
			}
			
				
			if(flag_voinuoc == 1)
			{
				Close(fileid_voinuoc);
				Signal("voinuoc");
				// Dung chuong trinh sinhvien lai de voi nuoc thuc thi
				Wait("sinhvien");
				
				// Tao file voinuoc.txt
				file_success = CreateFile("voinuoc.txt");
				if(file_success == -1)
				{
					Close(fileid_sinhvien);
					Signal("main"); // tro ve tien trinh chinh
					return;
				}
		

				// Mo file voinuoc.txt de ghi tung dung tich nuoc cua sinhvien
				fileid_voinuoc = Open("voinuoc.txt", 0);
				if(fileid_voinuoc == -1)
				{
					Close(fileid_sinhvien);
					Signal("main"); // tro ve tien trinh chinh
					return;
				}
				
			}
			i_File++;			
		}				
		// Ket thuc tien trinh sinhvien va voinuoc quay lai ham SvVn
		Signal("main");			
	}
	// Quay lai ham Svvn	
}
