#include "syscall.h"

char input[20] = "input.txt";
char output[20] = "output.txt";
char sinhvien_exe[20] = "sinhvien";
char voinuoc[10] = "voinuoc";
char tmp[10];
int pid[10];
int main() {
    int fileid1, n_sinhvien;
    int i = 0;

    CreateSemaphore(voinuoc, 1);
    fileid1 = Open(input, 0);
    CreateFile(output);
    Read(tmp, 1, fileid1);
    n_sinhvien = tmp[0] - '0';

    for (i = 0; i < n_sinhvien; ++i)
        pid[i] = Exec(sinhvien_exe);

    for (i = 0; i < n_sinhvien; ++i)
        Join(pid[i]);

    Close(fileid1);

    return 0;
}
