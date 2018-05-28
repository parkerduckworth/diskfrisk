#include <dirent.h>
// #include <errno.h> look into this
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


char* FILENAME = "Untitled.ipynb";

int frisk(char* dname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fstat;
    char path[PATH_MAX];
    int found = 0, len = strlen(dname);

    strcpy(path, dname);
    path[len++] = '/';
    
    if (!(dir = opendir(dname))) {
        perror("open");
        return -1;
    }

    while ((entry = readdir(dir))) {

        strcat(path, entry->d_name);
        if (!(strcmp(entry->d_name, ".")) || !(strcmp(entry->d_name, "..")))
            continue;

        strncpy(path + len, entry->d_name, PATH_MAX - len);
        lstat(path, &fstat);

        if (S_ISDIR(fstat.st_mode)) {
            frisk(path);
        } else if (strcmp(FILENAME, entry->d_name) == 0)
            printf("%s\n", path);
    }
    closedir(dir);


    return found;

}

int main()
{
    int res, frisk(char*);

    // print path to file 
    res = frisk("/Users");
    return res;
}