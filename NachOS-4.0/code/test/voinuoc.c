#include "syscall.h"

char inputFile[20] = "input.txt";
char outputFile[20] = "output.txt";
char sinvien_exec[20] = "sinhvien";
char semaphore[10] = "voinuoc";
char tmp[10];
int pid[10];
int main() {
    int file_id, n_sinhvien;
    int i = 0;

    CreateSemaphore(semaphore, 1);
    file_id = Open(inputFile, 0);
    CreateFile(outputFile);
    Read(tmp, 1, file_id);
    n_sinhvien = tmp[0] - '0';

    for (i = 0; i < n_sinhvien; ++i)
        pid[i] = Exec(sinvien_exec);

    for (i = 0; i < n_sinhvien; ++i)
        Join(pid[i]);

    Close(file_id);

    return 0;
}