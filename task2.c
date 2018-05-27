#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

void frisk(char* dir)
{
    DIR *dirptr = opendir(dir);
    struct dirent *entry;
    
    if (dirptr != NULL) {
        while (entry = readdir(dirptr))
            puts(entry->d_name);
        (void) closedir(dirptr);
    }
    else
        perror("Dir Error");

}

int main()
{
    void frisk(char*);

    frisk("./");

    return 0;
}