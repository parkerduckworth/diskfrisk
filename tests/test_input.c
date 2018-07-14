/* 
    Compile Tests:
        cc test_input.c -o test_execs/test_input -lcunit

    Execute Tests:
        test_execs/test_input
*/

#include <CUnit/Basic.h>
#include <stdio.h>
#include "test.h"
#include "../src/display.c"
#include "../src/diskfrisk.c"
#include "../src/extern.h"
#include "../src/sysdep.h"

extern int test;
extern int found;
extern char *dname;

char *pmatch_input;    // Test string representing pattern match input

int init_inputtest_suite(void)
{
    test = 1;

    t_option.t_csens = option.csens;
    t_option.t_pmatch = option.pmatch;
    t_option.t_home = option.home;
    t_option.t_openf = option.openf;
    t_option.t_perm = option.perm;
    t_option.t_sys = option.sys;

    // Test will attempt to set all options to 1, zero them out beforehand.
    option.csens = 0;
    option.pmatch = 0;
    option.home = 0;
    option.openf = 0;
    option.perm = 0;
    option.sys = 0;

    return 0;
}


int clean_inputtest_suite(void)
{
    // Reset previous config settings
    option.csens = t_option.t_csens;
    option.pmatch = t_option.t_pmatch;
    option.home = t_option.t_home;
    option.openf = t_option.t_openf;
    option.perm = t_option.t_perm;
    option.sys = t_option.t_sys;

    test = 0;

    return 0;
}


void dir_test(void)
{
    input(3, (char *[]) {NULL, "-s", TFNAME});
    CU_ASSERT_EQUAL(dname, ROOT);
    CU_ASSERT_EQUAL(option.sys, 1);
    option.sys = 0;

    input(3, (char *[]) {NULL, "-h", TFNAME});
    CU_ASSERT_EQUAL(dname, HOME);
    CU_ASSERT_EQUAL(option.home, 1);
    option.home = 0;
}


void input_flag_test(void)
{
    input(3, (char *[]) {NULL, "-o -e -C -p", TFNAME});
    CU_ASSERT_EQUAL(option.openf, 1);
    option.openf = 0;
    CU_ASSERT_EQUAL(option.perm, 1);
    option.perm = 0;
    CU_ASSERT_EQUAL(option.csens, 1);
    option.csens = 0;
    CU_ASSERT_EQUAL(option.pmatch, 1);
    option.pmatch = 0;
}


void error_flag_test(void)
{
    input(2, (char *[]) {NULL, "-?"});
    CU_ASSERT_EQUAL(error.bad_flag, 1);
    error.bad_flag = 0;
    CU_ASSERT_EQUAL(error.no_fn, 1);
    error.no_fn = 0;
}


void fname_test(void)
{
    CU_ASSERT(strcmp(input(2, (char *[]) {NULL, TFNAME}), TFNAME) == 0);
}

int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("input() Test Suite", init_inputtest_suite, clean_inputtest_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Directory Test", dir_test) ||
        (NULL == CU_add_test(pSuite, "Input Flag Test", input_flag_test)) ||
        (NULL == CU_add_test(pSuite, "Error Flag Test", error_flag_test)) ||
        (NULL == CU_add_test(pSuite, "Filename Input Test", fname_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
