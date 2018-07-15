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
    printf("%-25s [%s]\n", "Searching for:", fname);
    printf("%-25s %s\n", "Search type:", ((option.pmatch) ? "Pattern match" : "Exact match"));   
    printf("%-25s %s\n", "Case sensitivity:", ((option.csens) ? "On" : "Off"));
    printf("%-25s %s\n", "Search user files:", ((option.home) ? "On" : "Off"));
    printf("%-25s %s\n", "Search system files:", ((option.sys) ? "On" : "Off"));
    printf("%-25s %s\n", "Permission errors:", ((option.perm) ? "On" : "Off"));
    printf("%-25s %s\n", "Auto-open:", ((option.openf) ? "On" : "Off"));
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
