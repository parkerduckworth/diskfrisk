#ifndef EXTERN_H
#define EXTERN_H

#include <time.h>
#include "sysdep.h"

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
