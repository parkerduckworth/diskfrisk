#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "prototypes.h"
#include "sysdep.h"

#define CONFIG_PATH "/diskfrisk/config.json"


int main(int argc, char *argv[])
{
    char *fname;
    extern int found;
    extern char *dname;


    char *c_file_loc = build_cfile_path(CONFIG_PATH);
    set_config(c_file_loc);
    free(c_file_loc);

    if ((fname = input(argc, argv)) == NULL)
        return -1;

    frisk(fname, dname);

    return 0;
}
