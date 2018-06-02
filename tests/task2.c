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
#include <sys/wait.h>
#include <unistd.h>

#define ROOT "/"       // Root directory
#define USER "/Users"  // Home directory

int found = 0;


int main()
{
    int res;
    void frisk(char*, char*);

    frisk("envs", USER);

    return 0;
}


/* Call traverse, display result line */
void frisk(char* fname, char* dname)
{

    void traverse(char*, char*);

    traverse(fname, dname);

 // Add more to result line. Time elapsed, etc.
    printf("%d results.\n", found);

}


/* Traverse selected subtree  */
void traverse(char* fname, char* dname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fst;
    char path[PATH_MAX];
    int p_len = strlen(dname);  // Parent dir name length

    FILE *f;

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

        // Record absolute path and initialize fst
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fst);











        // Recurse if dir, else print matching result
        if (strcmp(fname, entry->d_name) == 0) {   
            printf("%s -> %s\n", fname, path);
            found++;
        } else if (S_ISDIR(fst.st_mode)) {
            traverse(fname, path);
        }




// Recurse if no match, else print matching result
        if (strcmp(fname, entry->d_name) == 0) {
            printf("%s -> %s\n", fname, path);

            // Open first occurance of filename match
            if (open) {
                if ((openfile(path)) < 0) {
                    printf("Unable to open %s\n", path);
                    exit(1);
                }

                // Must be set back to 0, or every result will be opened.
                open = 0;
            }
            found++;
        } else if (S_ISDIR(fst.st_mode)) {
            traverse(fname, path);
        } 









    }
    closedir(dir);
    return;
}



///////////////////////////////////////////////


/* TODO

-> Get task2 execvp sorted out for -o flag (waitpid(3) ?)
-> Allow search for dirs, if flagged, -o should work with this too
-> Maybe a GREP type search for not exact matches

*/




