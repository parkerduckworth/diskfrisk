/* 

Here is a short explanation of how to execute commands, with examples:

If no flags present, your machine's entire file tree is searched.
~ user$ ./frisk <filename>

To search the 'home' dir (user files) only, input the '-h' flag.
~ user$ ./frisk -h <filename>

If you want to search only system files, input '-s'.
~ user$ ./frisk -s <filename>

If both '-h' and '-s' are input, the result is the same as if no 
flags are input. They can also be input as '-sh' or '-hs'. Order
doesn't matter here.
~ user$ ./frisk -h -s <filename>

*/

#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define ROOT "/"           // Root directory
#define HOME "/Users"      // Home directory

char *input(int argc, char *argv[]);
void display_state(char c, char* fname);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);

char *dname = ROOT;

/* Input flags */
int sys = 0;       // Search all system files excluding home/user files
int home = 0;      // Search the home directory/user files

int no_fn = 0;     // No filename
int badflag = 0;   // Illegal flag
int found = 0;     // Number of results

clock_t start, end;
double t_elapsed;


int main(int argc, char *argv[])
{
    char *fname;

    if ((fname = input(argc, argv)) == NULL)
        return -1;

    frisk(fname, dname);
    
    if (found < 1)
        printf("%s not found...\n\n", fname);

    return 0;
}

/* Collect user input, parse optional flags */
char *input(int argc, char *argv[])
{
    char c, x = 0;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
                case 's':
                    sys = 1;
                    break;
                case 'h':
                    home = 1;
                    break;
                default:

                    // Save illegal flag to pass in error message
                    x = c;
                    badflag = 1;
                    break;
            }
    if (argc != 1)
        no_fn = 1;
    if (home && !sys)
        dname = HOME;

    // Update user on current state of process
    display_state(x, *argv);

    return (no_fn + badflag > 0) ? NULL : *argv;
}

/* Output current state to user */
void display_state(char c, char* fname)
{
    // Illegal input - error messages
    if (badflag)
        printf("\nIllegal option '%c'\n\n", c);
    else if (no_fn)
        printf("\nUsage: frisk -h -s <filename>\n\n");
        
    // Legal input submitted
    else {
        printf("\n\nDISKFRISK -- VERSION 0.0.0\n\n\n");
        if (sys || (!home && !sys))
            printf("System directory is being frisked...\n");
        if (home || (!home && !sys))
            printf("Home directory is being frisked...\n");
        printf("Searching for: %s\n\n", fname);
    }
}


/* Call traverse, display result line */
void frisk(char* fname, char* dname)
{
    start = clock();

    traverse(fname, dname);

    end = clock();
    t_elapsed = ((double)(end-start)) / CLOCKS_PER_SEC;

    printf("\n%d result%s, in %.2f seconds.\n\n", found, 
        ((found != 1) ? "s" : ""), t_elapsed);
}


/* Traverse selected subtree */
void traverse(char* fname, char* dname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fst;
    char path[PATH_MAX];
    size_t p_len = strlen(dname);  // Current absolute path length

    strcpy(path, dname);
    
    // Append to existing path string before adding child dir/file
    path[p_len++] = '/';

    // Not ideal, because no error catching for opendir()
    if (!(dir = opendir(dname)))
        return;

    while ((entry = readdir(dir))) {

        // These directories are not traversed, they're either hidden 
        // or require permission to open
        if (!(strcmp(entry->d_name, ".")) || !(strcmp(entry->d_name, "..")) ||
            entry->d_name[0] == '.' || !(strcmp(entry->d_name, "Guest")))
                continue;

        // If only "-s" is flagged
        if (!home && sys && !(strcmp(entry->d_name, "Users")))
                continue;

        // Record absolute path and initialize fst
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fst);

        // Recurse if dir, else print matching result
        if (S_ISDIR(fst.st_mode)) {
            traverse(fname, path);
        } else if (strcmp(fname, entry->d_name) == 0) {
            printf("%s -> %s\n", fname, path);
            found++;
        }
    }
    closedir(dir);
}