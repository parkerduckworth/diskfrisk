#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input(int argc, char *argv[]);

/* Input flags */
int grep = 0;
int home = 0;        // Search the home directory/user files
int open = 0;        // Open first occurance of filename match
int perm = 0;        // Display permission errors
int sys = 0;         // Search all system files excluding home/user files

/* Error flags */
int no_fn = 0;       // No filename
int badflag = 0;     // Illegal flag


int main(int argc, char *argv[])
{
    char *fname;

    if ((fname = input(argc, argv)) == NULL) {

        printf("\nMain returned: -1\n");
        return -1;
    }

    printf("\nMain returned: 0\n");
    return 0;
}


/* Collect user input, parse optional flags */
char *input(int argc, char *argv[])
{
    char c, x = 0;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
                 case 'h':
                    home = 1;
                    printf("OPTION: Searching home dir\n");
                    break;
                case 'o':
                    open = 1;
                     printf("OPTION: Opening file\n");
                    break;
                case 'p':
                    perm = 1;
                     printf("OPTION: Display perm errors\n");
                    break;
                case 's':
                    sys = 1;
                     printf("OPTION: Searching sys dir\n");
                    break;
                default:

                    // Save illegal flag to pass in error message
                    x = c;
                    badflag = 1;
                    printf("ERROR: Illegal flag\n");
                    break;
            }
    if (argc != 1) {
        no_fn = 1;
        printf("EFFOR: No filename input\n");
    } 

    if (strncmp(*argv, "grep:", 5) == 0) {
        grep = 1;
        printf("OPTION: grep\n");
        *argv = (*argv + 5);
    }

    printf("Text to search: %s\n", *argv);
    return (no_fn + badflag > 0) ? NULL : *argv;
}
