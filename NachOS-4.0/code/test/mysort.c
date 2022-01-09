/* mysort.c
 *	The program allows the user to input an array of n integers, and then prints to the console the sorted array.
 *
 */

#include "syscall.h"

int
main()
{
    int n, i, j, type, array[100], temp;

    PrintString("n = ", 10);
    n = ReadNum();

    if (n > 100 || n < 1)
        Halt();
    
    PrintString("Input array:\n", 20);
    for (i = 0; i < n; ++i)
    {
        array[i] = ReadNum();
    }

    PrintString("Input type sort (1: increase, -1: decrease): ", 50);
    type = ReadNum();

    if (type != 1 && type != -1)
        Halt();

    for (i = 0; i < n - 1; ++i)
    {
        for (j = 0; j < n - i - 1; ++j)
            if (type * (array[j] - array[j + 1]) > 0)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
    }

    PrintString("Result: ", 10);
    for (i = 0; i < n; ++i)
    {
        PrintNum(array[i]);
        PrintChar('\t');
    }

    Halt();
}
