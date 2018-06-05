/* TODO

-> Combine input flags into struct
-> Maybe a GREP type search for not exact matches

*/


///////////////////////////////////////////////////////////////////////////////


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

Directories can be searched, and set to open upon discovery as well.

An '-o' flag is available to automatically open up the first matched 
result of search.  The search will continue, and all matches will be
listed.

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
#include <sys/wait.h>
#include <unistd.h>

#define NULLCHAR  '\0'

/* OSX nomenclature */
#define HNAME     "Users"   // Home directory name
#define HOME      "/Users"  // Home directory path
#define ROOT      "/"       // Root directory path

/* Bash syntax */
#define RDFROMS   "-c"      // Read-from-string option
#define SHOPEN    "open "   // Open command
#define SHELL     "sh"      // Initiate new environment context

char *input(int argc, char *argv[]);
void display_state(char c, char *fname);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);
int entry_isvalid(char *fname);
void exec_result(char *fname, char *path);
int openfile(char *path);
int fork_process(char *sh_script, char *path);

/* Input flags */
int home = 0;        // Search the home directory/user files
int open = 0;        // Open first occurance of filename match
int perm = 0;        // Display permission errors
int sys = 0;         // Search all system files excluding home/user files

/* Error flags */
int no_fn = 0;       // No filename
int badflag = 0;     // Illegal flag

int found = 0;       // Number of results
char *dname = ROOT;  // Set by default

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
                 case 'h':
                    home = 1;
                    break;
                case 'o':
                    open = 1;
                    break;
                case 'p':
                    perm = 1;
                    break;
                case 's':
                    sys = 1;
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
void display_state(char c, char *fname)
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
        printf("Searching for: %s \n\n", fname);
    }
}


/* Call traverse, display result line */
void frisk(char *fname, char *dname)
{
    start = clock();

    traverse(fname, dname);

    end = clock();
    t_elapsed = ((double)(end-start)) / CLOCKS_PER_SEC;

    printf("\n%d result%s, in %.2f seconds.\n\n", found, 
        ((found != 1) ? "s" : ""), t_elapsed);
}


/* Traverse selected subtree */
void traverse(char *fname, char *dname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fst;
    char path[PATH_MAX];
    size_t p_len = strlen(dname);  // Current absolute path length

    strcpy(path, dname);
    
    // Append to existing path string before adding child dir/file
    path[p_len++] = '/';

    if (!(dir = opendir(dname))) {
        if (perm)
            printf("\nPermission denied: %s\n\n", path);
        return;
    }

    while ((entry = readdir(dir))) {

        if (!entry_isvalid(entry->d_name))
            continue;

        // Record absolute path and initialize fst
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fst);

        // Recurse if no match, else handle matching result
        if (strcmp(fname, entry->d_name) == 0) {
            exec_result(fname, path);

        } else if (S_ISDIR(fst.st_mode)) {
            traverse(fname, path);
        } 
    }
    closedir(dir);
}


/* Determine whether or not to traverse given entry */
int entry_isvalid(char *fname)
{
    int is_valid = 1;

    if (!(strcmp(fname, ".")) || !(strcmp(fname, "..")) ||
        fname[0] == '.')
            is_valid = 0;

    // If only "-s" is flagged
    if (!home && sys && !(strcmp(fname, HNAME)))
            is_valid = 0;

    return is_valid;
}


/* Execute input/filename match as necessary */
void exec_result(char *fname, char *path)
{
    printf("[%s] -> %s\n", fname, path);

    // Open first occurance of filename match
    if (open) {
        if ((openfile(path)) < 0) {
            printf("Unable to open %s\n", path);
        }

        // Must be set back to 0, or every result will be opened.
        open = 0;
    }
    found++;
    return;
}


/* Construct shell command to open target file */
int openfile(char *path)
{

    int res;
    char *sh_cmd = SHOPEN;          // Shell command, 'open'
    size_t curr_sz = strlen(path);  // Current length of shell script
    size_t c_len = strlen(sh_cmd);  // Length of shell command

    // String to contain shell script -> open command + filepath + slot for NULLCHAR
    char *sh_script = (char *)malloc(sizeof(char) * curr_sz + c_len + 1);

    // Udpate current size and terminate string.
    curr_sz += c_len + 1;
    *(sh_script + curr_sz) = NULLCHAR;

    strcpy(sh_script, sh_cmd);
    strcat(sh_script, path);

    res = fork_process(sh_script, path);

    free(sh_script);

    return res;
}


/* Execute shell command in forked child process */
int fork_process(char *sh_script, char *path)
{
    pid_t pid;
    int status;

    // Shell executable, separated into tokens
    char *sh_tok[] = {SHELL, RDFROMS, sh_script, NULL};

    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {

        // Child process
        printf("\nOpening: %s\n\n", path);
        if ((execvp(SHELL, sh_tok)) < 0) {
            perror("execvp");
            return -1;
        }
    } else {

        // Parent process
        while (wait(&status) != pid)
            ;
    }
    return 0;
}
