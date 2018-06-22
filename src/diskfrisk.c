/* TODO

-> Set up CI
-> Write remaining unit tests
-> Search for hidden files

*/

#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include "prototypes.h"
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

struct option_flags {
    int csens;       // Case-sensitive search
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

int test = 0;
int found = 0;       // Number of results
char *dname = ROOT;  // Set by default

clock_t start, end;
double t_elapsed;


/* Collect user input, parse optional flags */
char *input(int argc, char *argv[])
{
    char c, x = 1;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
                case 'C':
                    option.csens = 1;
                    break; 
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

    return (error.no_fn + error.bad_flag) ? NULL : *argv;
}


/* Output current state to user */
void display_state(char c, char *fname)
{
    if (!test) {

        // No errors, display initial state to user
        if (c == 1)
            initial_state(fname); 

        // Search complete
        else if (!c)
            result_line(fname); 

        else
            display_err(c);
    }
}


void initial_state(char *fname)
{
    printf("\n\nDISKFRISK -- VERSION 0.0.0\n\n\n");
    if (option.sys || (!option.home && !option.sys))
        printf("System directory is being frisked...\n");
    if (option.home || (!option.home && !option.sys))
        printf("Home directory is being frisked...\n");
    printf("Searching for%s: %s\n\n", ((option.grep) ? " pattern" : ""), fname);
}


void result_line(char *fname)
{
    printf("\n%d result%s, in %.2f seconds.\n\n", found, 
        ((found != 1) ? "s" : ""), t_elapsed);
    if (found < 1)
        printf("%s not found...\n\n", fname);
}


void display_err(char c)
{
    if (error.bad_flag)
        printf("\nIllegal option '%c'\n\n", c);
    else if (error.no_fn)
        printf("\nUsage: frisk -h -s <filename>\n\n");
}


/* Call traverse, display result line */
void frisk(char *fname, char *dname)
{
    start = clock();

    traverse(fname, dname);

    end = clock();
    t_elapsed = ((double)(end-start)) / CLOCKS_PER_SEC;

    display_state(NULLCHAR, fname);
}


/* Traverse selected subtree */
void traverse(char *fname, char *dname)
{
    char path[PATH_MAX];
    size_t p_len = strlen(dname);  // Current absolute path length
    DIR *dir;
    struct stat fst;
    struct dirent *entry;

    // Lets build this filepath
    strcpy(path, dname);
    path[p_len++] = '/';

    if (!(dir = opendir(dname))) {
        if (option.perm)
            printf("\nPermission denied: %s\n\n", path);
        return;
    }

    while ((entry = readdir(dir))) {
        if (!entry_isvalid(entry->d_name))
            continue;

        // Record absolute path and initialize stat struct
        strncpy(path + p_len, entry->d_name, PATH_MAX - p_len);
        lstat(path, &fst);

        if (compare_entry(fname, entry->d_name))
            process_match(fname, path);
        else if (S_ISDIR(fst.st_mode))
            traverse(fname, path);
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


/* Compare user input with current entry */
int compare_entry(char *fname, char *entry_name)
{  
    if (!option.csens) {
        char *cp;
        for (cp = fname; *cp; ++cp)
            *cp = tolower(*cp);
        for(cp = entry_name; *cp; ++cp)
            *cp = tolower(*cp);
    }

    if (option.grep)
        return (strstr(entry_name, fname) ? 1: 0);
    return (!strcmp(fname, entry_name) ? 1 : 0);
}


/* Execute input/filename match as necessary */
void process_match(char *fname, char *path)
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
    }

    // Child process
    else if (pid == 0) {  
        printf("\nOpening: %s\n\n", path);
        if ((execvp(SHELL, sh_tok)) < 0) {
            perror("execvp");
            return -1;
        }
    }

    // Wait to return to parent
    else {  
        while (wait(&status) != pid)
            ;
    }

    return 0;
}
