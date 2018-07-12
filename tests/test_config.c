/* 
    Compile Tests:
        cc test_config.c -o test_execs/test_config -lcunit

    Execute Tests:
        test_execs/test_config
*/

#include <CUnit/Basic.h>
#include "../src/config.c"
#include "../src/extern.h"
#include <stdio.h>

extern int test;
extern int found;

int truthify(FILE*);

char *cfile_loc = "../config.json";           // Original config file path
char *t_cfile_loc = "temp/test_config.json";  // Temp config file path

struct test_options {
    int t_csens;
    int t_grep;
    int t_home;
    int t_openf;
    int t_perm;
    int t_sys;
} t_option;

/* Copy config file to temp for testing */
int make_test_file(void)
{
    FILE *t_fp, *orig_fp; // Temp and original config files
    char c;

    if ((orig_fp = fopen(cfile_loc, "r")) == NULL) {
        perror("open");
        return 1;
    }
    if ((t_fp = fopen(t_cfile_loc, "w+")) == NULL) {
        perror("open");
        return 1;
    }

    while((c = getc(orig_fp)) != EOF)
        putc(c, t_fp);

    rewind(t_fp);
    truthify(t_fp);

    fclose(orig_fp);
    fclose(t_fp);

    return 0;
}

int truthify(FILE* fp)
{
    char *f_contents = pull_file(t_cfile_loc);
    char *to_replace = "false";
    char *replacement = "true ";

    char *pos;

    while ((pos = strstr(f_contents, to_replace)) != NULL)
        strncpy(pos, replacement, strlen(replacement));

    fprintf(fp, "%s", f_contents);
    free(f_contents);

    return 0;
}


int init_config_suite(void)
{
    // Save existing config settings
    t_option.t_csens = option.csens;
    t_option.t_grep = option.grep;
    t_option.t_home = option.home;
    t_option.t_openf = option.openf;
    t_option.t_perm = option.perm;
    t_option.t_sys = option.sys;

    // Test will attempt to set all options to 1, zero them out beforehand.
    option.csens = 0;
    option.grep = 0;
    option.home = 0;
    option.openf = 0;
    option.perm = 0;
    option.sys = 0;


    if (make_test_file() != 0)
        return -1;

    return 0;
}


int clean_config_suite(void)
{
    // Reset previous config settings
    option.csens = t_option.t_csens;
    option.grep = t_option.t_grep;
    option.home = t_option.t_home;
    option.openf = t_option.t_openf;
    option.perm = t_option.t_perm;
    option.sys = t_option.t_sys;

    if (remove(t_cfile_loc) != 0) {
        printf("Unable to remove %s\n", t_cfile_loc);
        return -1;
    }

    return 0;
}


void config_file_test(void)
{
    int ret;
    if ((ret = set_config(t_cfile_loc)) > 0)
        CU_FAIL("Invalid configuration file");

    CU_ASSERT_EQUAL(option.csens, 1);
    CU_ASSERT_EQUAL(option.grep, 1);
    CU_ASSERT_EQUAL(option.home, 1);
    CU_ASSERT_EQUAL(option.openf, 1);
    CU_ASSERT_EQUAL(option.perm, 1);
    CU_ASSERT_EQUAL(option.sys, 1);
}


int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("set_config() Test Suite", init_config_suite, clean_config_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Configuration File Parsing Test", config_file_test))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
