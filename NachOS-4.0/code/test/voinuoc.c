#include "syscall.h"

char inputFile[20] = "input.txt";
char outputFile[20] = "output.txt";
char sinhvien_execuatable[20] = "sinhvien";
char semaphore_name[10] = "voinuoc";
char tmp[10];
int pid[10];
int main() {
    int fd1, n_sinhvien;
    int i = 0;

    CreateSemaphore(semaphore_name, 1);
    fd1 = Open(inputFile, 0);
    CreateFile(outputFile);
    Read(tmp, 1, fd1);
    n_sinhvien = tmp[0] - '0';

    for (i = 0; i < n_sinhvien; ++i)
        pid[i] = Exec(sinhvien_execuatable);

    for (i = 0; i < n_sinhvien; ++i)
        Join(pid[i]);

    Close(fd1);

    return 0;
}
