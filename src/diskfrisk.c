/* TODO

-> Eliminate default case sensitivity, create '-C' flag as option instead
-> Set up CI

*/


///////////////////////////////////////////////////////////////////////////////


/* 

Here is a short explanation of how to execute commands, with examples:

If no flags present, your machine's entire file tree is searched.
Ex: ~ user$ ./frisk <filename>

To search the 'home' dir (user files) only, input the '-h' flag.
Ex: ~ user$ ./frisk -h <filename>

If you want to search only system files, input '-s'.
Ex: ~ user$ ./frisk -s <filename>

If both '-h' and '-s' are input, the result is the same as if no 
flags are input. They can also be input as '-sh' or '-hs'. Order
doesn't matter here.
Ex: ~ user$ ./frisk -h -s <filename>

Directories can be searched, and set to open upon discovery as well.

An '-o' flag is available to automatically open up the first matched 
result of search.  The search will continue, and all matches will be
listed.

A '-p' flag is another option that sets all file permission errors to
display when thrown.

If you wish to search by pattern, rather than by exact match, preface
your <filename> with 'grep:', and all files containing the matching
pattern will be output. This can be used in conjuction with any (or all)
of the optional flags.
Ex: ~ user$ frisk grep:<filename>

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
#include "sysdep.h"
#include <sys/wait.h>
#include <unistd.h>

#define NULLCHAR  '\0'
#define PMATCH    "grep:"         // User command to search by pattern match
#define PM_LEN    strlen(PMATCH)  // Pattern match command length

char *input(int argc, char *argv[]);
void display_state(char c, char *fname);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);
int entry_isvalid(char *fname);
void pmatch(char *fname, char *text, char *path);
void exec_result(char *fname, char *path);
int openfile(char *path);
int fork_process(char *sh_script, char *path);

struct option_flags {
    int grep;        // Search by pattern match
    int home;        // Search the home directory/user files
    int openf;       // Open first occurance of filename match
    int perm;        // Display permission errors
    int sys;         // Search all system files excluding home/user files
} option;

struct error_flags {
    int no_fn;       // No filename
    int bad_flag;    // Illegal flag
} error;

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
                    option.home = 1;
                    break;
                case 'o':
                    option.openf = 1;
                    break;
                case 'p':
                    option.perm = 1;
                    break;
                case 's':
                    option.sys = 1;
                    break;
                default:

                    // Save illegal flag to pass in error message
                    x = c;
                    error.bad_flag = 1;
                    break;
            }
    if (argc != 1)
        error.no_fn = 1;
    if (option.home && !option.sys)
        dname = HOME;
    if (!strncmp(*argv, PMATCH, PM_LEN)) {
        option.grep = 1;
        *argv = (*argv + PM_LEN);
    }

    // Update user on current state of process
    display_state(x, *argv);

    return (error.no_fn + error.bad_flag > 0) ? NULL : *argv;
}


/* Output current state to user */
void display_state(char c, char *fname)
{
    // Illegal input - error messages
    if (error.bad_flag)
        printf("\nIllegal option '%c'\n\n", c);
    else if (error.no_fn)
        printf("\nUsage: frisk -h -s <filename>\n\n");
        
    // Legal input submitted
    else {
        printf("\n\nDISKFRISK -- VERSION 0.0.0\n\n\n");
        if (option.sys || (!option.home && !option.sys))
            printf("System directory is being frisked...\n");
        if (option.home || (!option.home && !option.sys))
            printf("Home directory is being frisked...\n");
        printf("Searching for%s: %s\n\n", ((option.grep) ? " pattern" : ""), fname);
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
        if (option.perm)
            printf("\nPermission denied: %s\n\n", path);
        return;
    }

    while ((entry = readdir(dir))) {

        if (!entry_isvalid(entry->d_name))
            continue;

        // Record absolute path and initialize fst
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fst);

        // User selected grep option, determine if file contains pattern
        if (option.grep)
            pmatch(fname, entry->d_name, path);

        // Recurse if no match, else handle matching result
        if (!option.grep && !strcmp(fname, entry->d_name)) {
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
    if (!option.home && option.sys && !(strcmp(fname, HNAME)))
            is_valid = 0;

    return is_valid;
}


/* Display results that match input pattern  */
void pmatch(char *currfile, char *text, char *path)
{
    if (strstr(text, currfile)) {
        exec_result(currfile, path);
    }
}


/* Execute input/filename match as necessary */
void exec_result(char *fname, char *path)
{
    printf("[%s] -> %s\n", fname, path);

    // Open first occurance of filename match
    if (option.openf) {
        if ((openfile(path)) < 0) {
            printf("Unable to open %s\n", path);
        }

        // Must be set back to 0, or every result will be opened.
        option.openf = 0;
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
