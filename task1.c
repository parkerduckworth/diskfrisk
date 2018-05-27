#include <stdio.h>

int OPEN = 0;   // input flag - open first first with matching filename
int SYS = 0;    // input flag - search all system files excluding home/user files
int USR = 0;    // input flag - search the home directory and user files

int main(int argc, char *argv[])
{
    int c;

    while (--argc > 0 && (*++argv)[0] == '-')
        while (c = *++argv[0])
            switch (c) {
            case 'o':
                OPEN = 1;
                break;
            case 's':
                SYS = 1;
                break;
            case 'u':
                USR = 1;
                break;
            default:
                printf("diskfrisk: illegal option %c\n", c);
                argc = 0;
                break;
            }
    if (argc != 1)
        printf("Usage: frisk -su filename\n");
    else
        // TODO appropriate dir's need to be triggered by SYS and USR,
        //      whether inluded together or individually.
        if (OPEN)
            printf("file opened\n");
        if (SYS)
            printf("system files searched\n");
        if (USR)
            printf("user files searched\n");
    return 0;
}