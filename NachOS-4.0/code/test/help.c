/* help.c
 * Gioi thieu ve nhom va cac file sort cung nhu ascii
 */

#include "syscall.h"

int main() {
    char* teamIntro = "------Gioi thieu nhom------\n";
    char* school = "DH KHTN - DHQG TP.HCM\n";
    char* class = "19CNTN\n";
    char* team =
        "Doan Kim Huy - 19120239\n"
        "Vo Chanh Hung - 19120523\n"
        "Quach Binh Long - 19120283\n";

    char* slash = "----------------------------------------------\n";

    char* sortIntroduction =
        "Chuong trinh Sort cho phep sap xep mot mang so nguyen co so phan tu "
        "nho hon 100.\n";
    char* sortUsage =
        "Cach su dung:\n"
        "1. Nhap so phan tu cua mang nho hon 100.\n"
        "2. Nhap cac phan tu cua mang.\n"
        "3. Chon sap xep tang dan hoac giam dan.\n"
        "4. Sap xep mang bang thuat toan Bubble Sort.\n";

    char* asciiIntroduction =
        "Chuong trinh Ascii in ra danh sach cac ky tu thuoc bang ma ascii.\n";

    // Gioi thieu nhom
    PrintString(teamIntro, 255);
    PrintString(school, 255);
    PrintString(class, 255);
    PrintString(team, 255);

    PrintString(slash, 5);

    // Gioi thieu chuong trinh Sort
    PrintString(sortIntroduction, 255);
    PrintString(sortUsage, 255);

    PrintString(slash, 255);

    // Gioi thieu chuong trinh Ascii
    PrintString(asciiIntroduction, 255);

    Halt();
    /* not reached */
}