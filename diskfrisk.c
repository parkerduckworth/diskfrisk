#include <ctype.h>
#include <dirent.h>
// #include <errno.h> look into this
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOT "/"           // Root directory
#define USER "/Users"      // Home directory

char *input(int argc, char *argv[]);
int frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);

char *dname = ROOT;

/* Input flags */
int open = 0;   // Open first result with matching filename
int sys = 0;    // Search all system files excluding home/user files
int usr = 0;    // Search the home directory/user files

int found = 0;


int main(int argc, char *argv[])
{
    char *fname;
    int res;

    fname = input(argc, argv);
    if ((res = frisk(fname, dname)) < 0)
        printf("%s not found...\n", fname);

    // Delete this test
    printf("Main returned: %d\n", res);

    return res;
}


char *input(int argc, char *argv[])
{
    int c;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
            case 'o':
                open = 1;
                break;
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
        printf("Usage: frisk -s -u filename\n");
    else
        if (usr && !sys)
            dname = USER;

    /* Print test begin */
        if (open)
            printf("file opened\n");
        if (sys)
            printf("system files searched\n");
        if (usr)
            printf("user files searched\n");
    printf("\n%s\n", *argv);
    printf("\ndir_to_s: %s\n\n", dname);
    /* Print test end */

    return *argv;
}


int frisk(char* fname, char* dname)
{
    void traverse(char*, char*);

    traverse(fname, dname);

    // Add more to result line. Time elapsed, etc.
    printf("%d results.\n", found);
    
    return (found > 0) ? 0 : -1;
}


/* Traverse selected subtree  */
void traverse(char* fname, char* dname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fstat;
    char path[PATH_MAX];
    int p_len = strlen(dname);  // Parent dir name length

    strcpy(path, dname);
    path[p_len++] = '/';
    
    // printf("%s\n", dname); // delete this?

    // Hidden files at end of User/~ dir require permissions, throws segfault
    // Not ideal, because no error catching for opendir().
    if (!(dir = opendir(dname)))
        // perror("opendir");
        return;

    while ((entry = readdir(dir))) {

        // strcpy(path, entry->d_name); //probably dont need this

        // What are these ".", "..", and ".dirname" files
        if (!(strcmp(entry->d_name, ".")) || !(strcmp(entry->d_name, "..")) || 
            entry->d_name[0] == '.' || !(strcmp(entry->d_name, "Guest")))
                continue;

        // Record absolute path and populate fstat
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fstat);

        // Recurse if dir, else print matching result
        if (S_ISDIR(fstat.st_mode)) {
            traverse(fname, path);
        } else if (strcmp(fname, entry->d_name) == 0) {   
            printf("%s -> %s\n", fname, path);
            found++;
        }
    }
    closedir(dir);
    return;
}