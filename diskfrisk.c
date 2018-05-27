#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define ROOT "/"
#define USER "/Users/"

char *input(int argc, char *argv[]);
int frisk(char*);

char *dir_to_search = ROOT;
int OPEN = 0;   // input flag - open first first with matching filename
int SYS = 0;    // input flag - search all system files excluding home/user files
int USR = 0;    // input flag - search the home directory and user files


int main(int argc, char *argv[])
{
    char *file;
    int res;

    file = input(argc, argv);
    if ((res = frisk(file)) < 0)
        perror("Not Found");
    return res;
}


char *input(int argc, char *argv[])
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
        printf("Usage: frisk -s -u filename\n");
    else
        if (USR && !SYS)
            dir_to_search = USER;

        /* Print test begin */
        if (OPEN)
            printf("file opened\n");
        if (SYS)
            printf("system files searched\n");
        if (USR)
            printf("user files searched\n");
    printf("\n%s\n", *argv);
    printf("\ndir_to_s: %s\n\n", dir_to_search);
    /* Print test end*/

    return *argv;
}


int frisk(char* file)
{
    DIR *dirptr = opendir(dir_to_search);
    struct dirent *entry;
    int found = 0;
    
    if (dirptr != NULL) {
        while ((entry = readdir(dirptr)) != NULL) {
            puts(entry->d_name);
            // if (strcmp(file, entry->d_name) == 0) {
            //     puts(entry->d_name);
            //     found++;
            // }
            // else
            //     continue;
        }
        closedir(dirptr);
    }
    else
        perror("Dir Error");

    return (found > 0) ? found : -1;

}