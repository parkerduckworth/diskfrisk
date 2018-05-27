#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int frisk(char* dir, char* file)
{
    DIR *dirptr = opendir(dir);
    struct dirent *entry;
    int found = 0;
    
    if (dirptr != NULL) {
        while ((entry = readdir(dirptr)) != NULL) {
            if (strcmp(file, entry->d_name) == 0) {
                puts(entry->d_name);
                found++;
            }
            else
                continue;
        }
        closedir(dirptr);
    }
    else
        perror("Dir Error");

    return (found > 0) ? found : -1;

}

int main()
{
    int res, frisk(char*, char*);

    res = frisk("./", "filename.png");
    if (res < 0)
        printf("File not found...\n");
    return res;
}