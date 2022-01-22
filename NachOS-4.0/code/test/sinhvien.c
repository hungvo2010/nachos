#include "syscall.h"

char outputFile[20] = "output.txt";
char semaphore[10] = "voinuoc";
char tmp[10];
char buffer[3] = "  ";
int main() {
    int file_id;
    int pid;
    int current_amount = 0;
    int i = 0;
    int random_number;
    pid = GetPID();

    while (current_amount < 10) {
        Wait(semaphore);
        file_id = Open(outputFile, 2);

        buffer[0] = (char)('0' + pid);
        random_number = RandomNum() % 10000;
        for (i = 0; i < random_number; ++i)
            ;
        Write(buffer, 2, file_id);
        current_amount++;

        Close(file_id);
        Signal(semaphore);
    }
    return 0;
}