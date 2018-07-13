#ifndef PROTOTYPES_H
#define PROTOTYPES_H

/* main.c, diskfrisk.c -> diskfrisk.h*/
char* input(int argc, char *argv[]);
void frisk(char *fname, char *dname);
void traverse(char *fname, char *dname);
char *build_path(char *path, char *base_dir);
int openfile(char *path);
int fork_process(char *sh_script, char *path);
void display_state(char c, char *fname);

/* main.c, config.c, test_config.c -> config.h*/
char *build_cfile_path(char *path);
int set_config(char *c_file);
char* pull_file(char *fname);

#endif
