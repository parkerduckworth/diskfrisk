#ifndef EXTERN_H
#define EXTERN_H

#include <time.h>
#include "sysdep.h"

    #ifndef CONFIG_PATH
    #define CONFIG_PATH "/diskfrisk/config.json"
    #endif

char* input(int argc, char *argv[]);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);
int openfile(char *path);
void display_state(char c, char *fname);
char *build_cfile_path(char *path);
int set_config(char *c_file);
char* pull_file(char *fname);

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

int test;            // CUnit hook
int found;           // Number of results
char *dname;         // Set by default

clock_t start, end;
double t_elapsed;

#endif
