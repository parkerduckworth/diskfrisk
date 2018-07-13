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
    if (option.grep)
        printf("Searching for pattern: %s\n", fname);
    else
        printf("Searching for exact match: %s\n", fname);
    if (option.sys)
        printf("System directory is being frisked...\n");
    if (option.home)
        printf("Home directory is being frisked...\n");
    if (option.openf)
        printf("Opening first match...\n");
    if (option.csens)
        printf("Case sensitive search...\n");
    if (option.perm)
        printf("Permission errors activated...\n");
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
