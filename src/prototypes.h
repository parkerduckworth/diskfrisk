#ifndef PROTOTYPES_H_INCLUDED
#define PROTOTYPES_H_INCLUDED

char *input(int argc, char *argv[]);
void display_state(char c, char *fname);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);
int entry_isvalid(char *fname);
int casecmp(char *, char *);
void pmatch(char *fname, char *text, char *path);
void exec_result(char *fname, char *path);
int openfile(char *path);
int fork_process(char *sh_script, char *path);

#endif