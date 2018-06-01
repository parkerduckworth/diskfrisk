/* FIGURE OUT HOW TO DEBUG THIS BLOODY THING */

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/wait.h>
// #include <unistd.h>

// #define NULLCHAR  '\0'
// #define RDFROMS   "-c"     // Bash - "read from string" option
// #define SHOPEN    "open "  // Bash - open command
// #define SHELL     "sh"     // Bash - initiate new environment context

#define TESTPATH  "/Users/parkerduckworth/3CSS"  


int openfile(char* path);
int fork_process(char *sh_script, char *path);


int main()
{
    int res;

    char* path = TESTPATH;

    res = openfile(path);

    return res;
}



/*  */
int openfile(char* path)
{

    int res;
    char *sh_cmd = SHOPEN;          // Shell command, 'open'
    size_t curr_sz = strlen(path);  // Current length of shell script
    size_t c_len = strlen(sh_cmd);  // Length of shell command

    // String to contain shell script -> open command + filepath + slot for NULLCHAR
    char *sh_script = (char*)malloc(sizeof(char) * curr_sz + c_len + 1);

    // Udpate current size and terminate string.
    curr_sz += c_len + 1;
    *(sh_script + curr_sz) = NULLCHAR;

    strcpy(sh_script, sh_cmd);
    strcat(sh_script, path);

    res = fork_process(sh_script, path);

    free(sh_script);
    printf("char* script has been deallocated.\n\n");  // Test deallocation

    return res;
}

int fork_process(char* sh_script, char* path)
{
    pid_t pid;
    int status;

    // Shell executable, separated into tokens
    char *sh_tok[] = {SHELL, RDFROMS, sh_script, NULL};

    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {

        // Child process
        printf("\nOpening: %s\n\n", path);
        if ((execvp(SHELL, sh_tok)) < 0) {
            perror("execvp");
            return -1;
        }
    } else {

        // Parent process
        while (wait(&status) != pid)
            ;
    }
    return 0;
}
