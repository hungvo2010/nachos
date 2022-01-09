#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

void SysHalt();
int SysAdd(int op1, int op2);
void SysPrintNum(int n);
int SysReadNum();
char SysReadChar();
void SysPrintChar(char c);
int SysRandomNum();
int SysPrintString(int buffer_address, int length);
int SysReadString(int buffer_address, int length);
int SysCreatefile(char* filename);
int SysOpenfile(char* filename, int type);
int SysClosefile(int file_id);
int SysExec(char* filename);
int SysReadfile(int file_id, int virtual_address, int size);
int SysWritefile(int file_id, int virtual_address, int size);

char *User2System(int virtAddr, int limit);
int System2User(int virtAddr, int len, char *buffer);


#endif /* ! __USERPROG_KSYSCALL_H__ */
