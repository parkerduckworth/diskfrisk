#include "prototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include "sysdep.h"


int main(int argc, char *argv[])
{
    char *fname;
    extern int found;
    extern char *dname;

    if ((fname = input(argc, argv)) == NULL)
        return -1;

    frisk(fname, dname);

    return 0;
}
