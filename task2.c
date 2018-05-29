/* FIGURE OUT HOW TO DEBUG THIS BLOODY THING */


#include <ctype.h>
#include <dirent.h>
// #include <errno.h> look into this
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOT "/"       // Root directory
#define USER "/Users"  // Home directory

int found = 0;


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

int frisk(char* fname, char* dname)
{
    void traverse(char*, char*);

    traverse(fname, dname);

    // Add more to result line. Time elapsed, etc.
    printf("%d results.\n", found);
    
    return (found > 0) ? 0 : -1;
}


int main()
{
    int res, frisk(char*, char*);

    res = frisk("LES.pdf", USER);

    printf("Main returned: %d\n", res);
    return res;
}
