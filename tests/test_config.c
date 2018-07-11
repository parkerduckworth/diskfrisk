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

#define TFNAME   "TEST"             // <filename> used for testing

extern int test;
extern int found;

FILE *t_config_file;
char *cfile_loc = "../config.json";  // path to config file
char *test_cfile_loc = "temp/test_config.json";

int test_csens;
int test_grep;
int test_home;
int test_openf;
int test_perm;
int test_sys;

int make_test_file()
{
    FILE *orig;
    char c;

    if ((orig = fopen(cfile_loc, "r")) == NULL) {
        perror("open");
        return 1;
    }
    if ((t_config_file = fopen(test_cfile_loc, "w")) == NULL) {
        perror("open");
        return 1;
    }

    while((c = getc(orig)) != EOF)
        putc(c, t_config_file);

    fclose(orig);
    fclose(t_config_file);

    return 0;
}


int init_config_suite(void)
{
    test_csens = option.csens;
    test_grep = option.grep;
    test_home = option.home;
    test_openf = option.openf;
    test_perm = option.perm;
    test_sys = option.sys;

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

void truthify(void)  // Find first occurance of false and change to true
{
    return;
}


int clean_config_suite(void)
{
    option.csens = test_csens;
    option.grep = test_grep;
    option.home = test_home;
    option.openf = test_openf;
    option.perm = test_perm;
    option.sys = test_sys;

    if (remove(test_cfile_loc) != 0) {
        printf("Unable to remove %s\n", test_cfile_loc);
        return -1;
    }

    return 0;
}


void config_file_test(void)
{
    int res;
    if ((res = set_config(test_cfile_loc)) > 0)
        CU_FAIL("Invalid configuration file");

    // Write to test_config.json, change all false -> true

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
