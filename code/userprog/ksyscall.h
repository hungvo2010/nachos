#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

void SysHalt();
int SysAdd(int op1, int op2);
void Sys_PrintNum(int n);
int Sys_ReadNum();
char Sys_ReadChar();
void Sys_PrintChar(char c);
int Sys_RandomNum();
int Sys_PrintString(int buffer_address, int length);
int Sys_ReadString(int buffer_address, int length);
int Sys_Createfile(char* filename);
int Sys_Openfile(char* filename, int type);
int Sys_Closefile(int file_id);
int Sys_Exec(char* filename);
int Sys_Readfile(int file_id, int virtual_address, int size);
int Sys_Writefile(int file_id, int virtual_address, int size);

char *User2System(int virtAddr, int limit);
int System2User(int virtAddr, int len, char *buffer);


#endif /* ! __USERPROG_KSYSCALL_H__ */
