#include <stdlib.h>
#include "extern.h"
#include "display.c"
#include "diskfrisk.c"
#include "config.c"

unsigned int open_settings(void)
{
    char *c_file_loc = build_cfile_path(CONFIG_PATH);

    if ((openfile(c_file_loc)) < 0) {
        printf("Unable to open settings\n");
        return 1;
    }

    free(c_file_loc);
    return 0;
}

int main()
{
    if (open_settings() > 1)
        return -1;
    return 0;
}