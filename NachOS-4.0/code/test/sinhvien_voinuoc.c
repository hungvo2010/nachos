#include "syscall.h"
#include "copyright.h"

int main()
{

	int file_success;												 // result of file related operation
	int fileid_input, fileid_output, fileid_sinhvien, fileid_result; // some file id needed
	int SLTD;														 // number of test cases
	char c_readFile;												 // character read from file

	// initialize 4 semaphore
	file_success = CreateSemaphore("main", 0);
	if (file_success == -1)
		return 1;
	file_success = CreateSemaphore("sinhvien", 0);
	if (file_success == -1)
		return 1;
	file_success = CreateSemaphore("voinuoc", 0);
	if (file_success == -1)
		return 1;
	file_success = CreateSemaphore("m_vn", 0);
	if (file_success == -1)
		return 1;

	// create file output.txt
	file_success = CreateFile("../test/output.txt");
	// if (file_success == -1)
	// 	return 1;

	// open file input.txt	for read only
	fileid_input = Open("../test/input.txt", 1);
	if (fileid_input == -1)
		return 1;

	// open file output.txt de read and write
	fileid_output = Open("../test/output.txt", 0);
	if (fileid_output == -1)
	{
		Close(fileid_input);
		return 1;
	}

	// read SLTD from file input.txt
	//**** after that file pointer of input.txt at line 1
	SLTD = 0;
	// read digit by digit
	while (1)
	{
		Read(&c_readFile, 1, fileid_input);
		PrintChar(c_readFile);
		if (c_readFile != '\n')
		{
			if (c_readFile >= '0' && c_readFile <= '9')
				SLTD = SLTD * 10 + (c_readFile - 48);
		}
		else
			break;
	}
	// Goi thuc thi tien trinh sinhvien.c
	file_success = Exec("../test/sinhvien");
	PrintString("execsuccess");
	if (file_success == -1)
	{
		Close(fileid_input);
		Close(fileid_output);
		return 1;
	}

	// Goi thuc thi tien trinh voinuoc.c
	file_success = Exec("../test/voinuoc");
	if (file_success == -1)
	{
		Close(fileid_input);
		Close(fileid_output);
		return 1;
	}

	while (SLTD--)
	{
		// Tao file sinhvien.txt
		file_success = CreateFile("../test/sinhvien.txt");
		if (file_success == -1)
		{
			Close(fileid_input);
			Close(fileid_output);
			return 1;
		}

		// Mo file sinhvien.txt de ghi tung dong sinhvien tu file input.txt
		fileid_sinhvien = Open("../test/sinhvien.txt", 0);
		if (fileid_sinhvien == -1)
		{
			Close(fileid_input);
			Close(fileid_output);
			return 1;
		}
		while (1)
		{
			if (Read(&c_readFile, 1, fileid_input) < 1)
			{
				// Doc toi cuoi file
				break;
			}
			if (c_readFile != '\n')
			{
				Write(&c_readFile, 1, fileid_sinhvien);
			}
			else
				break;
		}
		// Dong file sinhvien.txt lai
		Close(fileid_sinhvien);

		// Goi tien trinh sinhvien hoat dong
		Signal("sinhvien");

		// Tien trinh chinh phai cho
		Wait("main");

		// Thuc hien doc file tu result va ghi vao ket qua o output.txt
		fileid_result = Open("../test/result.txt", 1);
		if (fileid_result == -1)
		{
			Close(fileid_input);
			Close(fileid_output);
			return 1;
		}

		PrintString("\n Lan thu: ");
		PrintNum(SLTD);
		PrintString("\n");

		// Doc cac voi vao output.txt
		while (1)
		{
			if (Read(&c_readFile, 1, fileid_result) < 1)
			{
				Write("\r\n", 2, fileid_output);
				Close(fileid_result);
				Signal("m_vn");
				break;
			}
			Write(&c_readFile, 1, fileid_output);
			Write(" ", 1, fileid_output);
		}
	}

	Close(fileid_input);
	Close(fileid_output);
	return 0;
}