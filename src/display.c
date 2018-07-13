#include <stdio.h>
#include "extern.h"

extern int test;
extern int found;

static void initial_state(char *fname);
static void result_line(char *fname);
static void display_err(char c);

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


static void initial_state(char *fname)
{
    printf("\n\nDISKFRISK -- VERSION 0.0.0\n\n\n");
    printf("Searching for: [%s]\n", fname);
    printf("Search type: %s\n", ((option.grep) ? "Pattern match" : "Exact match"));    
    printf("Case sensitivity: %s\n", ((option.csens) ? "On" : "Off"));
    printf("Search user files: %s\n", ((option.home) ? "On" : "Off"));
    printf("Search system files: %s\n", ((option.sys) ? "On" : "Off"));
    printf("Permission errors: %s\n", ((option.perm) ? "On" : "Off"));
    printf("Auto-open: %s\n", ((option.openf) ? "On" : "Off"));
    printf("\n");
}


static void result_line(char *fname)
{
    printf("\n%d result%s, in %.2f seconds.\n\n", found, 
        ((found != 1) ? "s" : ""), t_elapsed);
    if (found < 1)
        printf("%s not found...\n\n", fname);
}


static void display_err(char c)
{
    if (error.bad_flag)
        printf("\nIllegal option '%c'\n\n", c);
    else if (error.no_fn)
        printf("\nEnter a file name\n\n");
}
