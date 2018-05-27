#include <stdio.h>

int main(int argc, char *argv[])
{
    int c, open = 0, sys = 0, usr = 0;

    while (--argc > 0 && (*++argv)[0] == '-')
        while (c = *++argv[0])
            switch (c) {
            case 'o':
                open = 1;
            case 's':
                sys = 1;
                break;
            case 'u':
                usr = 1;
                break;
            default:
                printf("diskfrisk: illegal option %c\n", c);
                argc = 0;
                break;
            }
    if (argc != 1)
        printf("Usage: frisk -su filename");
    else
        ; //execute search

    return 0;
}