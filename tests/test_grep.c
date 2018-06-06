#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NULLCHAR  '\0'

/* OSX nomenclature */
#define HNAME     "Users"         // Home directory name
#define HOME      "/Users"        // Home directory path
#define ROOT      "/"             // Root directory path

/* Bash syntax */
#define RDFROMS   "-c"            // Read-from-string option
#define SHOPEN    "open "         // Open command
#define SHELL     "sh"            // Initiate new environment context

/* Test Definitions */
#define TESTDIR   "/Users"
#define PMATCH    "grep:"         // User command to search by pattern match
#define PM_LEN    strlen(PMATCH)  // Pattern match command length

char *input(int argc, char *argv[]);
void traverse(char *fname, char *dname);
void pmatch(char *fname, char *text, char *path);
int entry_isvalid(char *fname);
void exec_result(char *fname, char *path);
int openfile(char *path);
int fork_process(char *sh_script, char *path);

/* Input flags */
int grep = 0;
int home = 0;        // Search the home directory/user files
int open = 0;        // Open first occurance of filename match
int perm = 0;        // Display permission errors
int sys = 0;         // Search all system files excluding home/user files

/* Error flags */
int no_fn = 0;       // No filename
int badflag = 0;     // Illegal flag

int found = 0;

int main(int argc, char *argv[])
{
    char *fname;

    if ((fname = input(argc, argv)) == NULL) {

        printf("\nMain returned: -1\n");
        return -1;
    }

    traverse(fname, TESTDIR);
    printf("%d results\n", found);

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

    if (!strncmp(*argv, PMATCH, PM_LEN)) {
        grep = 1;
        printf("OPTION: grep\n");
        *argv = (*argv + PM_LEN);
    }

    printf("Text to search: %s\n", *argv);
    return (no_fn + badflag > 0) ? NULL : *argv;
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

        // User selected grep option, determine if file contains pattern
        if (grep)
            pmatch(fname, entry->d_name, path);

        // Recurse if no match, else handle matching result
        if (!grep && !strcmp(fname, entry->d_name)) {
            exec_result(fname, path);
            found ++;

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


/* Display results that match input pattern  */
void pmatch(char *currfile, char *text, char *path)
{
    if (strstr(text, currfile)) {
        exec_result(currfile, path);
        found++;
    }
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
