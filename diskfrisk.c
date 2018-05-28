#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define ROOT "/"            // Root directory
#define USER "/Users/"      // Home directory

char *input(int argc, char *argv[]);
int frisk(char*);

char *DNAME = ROOT;

/* Input flags */
int OPEN = 0;   // Open first result with matching filename
int SYS = 0;    // Search all system files excluding home/user files
int USR = 0;    // Search the home directory/user files


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
            DNAME = USER;

    /* Print test begin */
        if (OPEN)
            printf("file opened\n");
        if (SYS)
            printf("system files searched\n");
        if (USR)
            printf("user files searched\n");
    printf("\n%s\n", *argv);
    printf("\ndir_to_s: %s\n\n", DNAME);
    /* Print test end */

    return *argv;
}


int frisk(char* file)
{
    DIR *dir = opendir(DNAME);
    struct dirent *entry;
    int found = 0;
    
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            puts(entry->d_name);
            // if (strcmp(file, entry->d_name) == 0) {
            //     puts(entry->d_name);
            //     found++;
            // }
            // else
            //     continue;
        }
        closedir(dir);
    }
    else
        perror("Dir Error");

    return (found > 0) ? found : -1;

}
