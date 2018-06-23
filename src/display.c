#include "extern.h"
#include "prototypes.h"
#include <stdio.h>

extern int test;
extern int found;

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
        printf("\nUsage: find -h -s <filename>\n\n");
}
