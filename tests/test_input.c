/* 
    Compile Tests:
        cc test_input.c -o test_execs/test_input -lcunit

    Execute Tests:
        test_execs/test_input
*/

#include <CUnit/Basic.h>
#include "../src/diskfrisk.c"
#include "../src/prototypes.h"
#include "../src/sysdep.h"
#include <stdio.h>

#define TFNAME "TEST"  // <filename> used for testing

char *pmatch_input;    // Test string representing pattern match input

int init_inputtest_suite(void)
{
    pmatch_input = malloc(sizeof(char) * (PM_LEN + strlen(TFNAME) + 1));
    strcat(pmatch_input, PMATCH);
    strcat(pmatch_input, TFNAME);
    *(pmatch_input + PM_LEN + strlen(TFNAME) + 1) = NULLCHAR;

    test = 1;
    
    return 0;
}


int clean_inputtest_suite(void)
{
    option.csens = 0;
    option.grep = 0;
    option.home = 0;
    option.openf = 0;
    option.perm = 0;
    option.sys = 0;
    error.bad_flag = 0;
    error.no_fn = 0;

    free(pmatch_input);
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
    input(3, (char *[]) {NULL, "-o -p -C", pmatch_input});
    CU_ASSERT_EQUAL(option.openf, 1);
    option.openf = 0;
    CU_ASSERT_EQUAL(option.perm, 1);
    option.perm = 0;
    CU_ASSERT_EQUAL(option.csens, 1);
    option.csens = 0;
    CU_ASSERT_EQUAL(option.grep, 1);
    option.grep = 0;
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
    CU_ASSERT(strcmp(input(2, (char *[]) {NULL, pmatch_input}), TFNAME) == 0);
    option.grep = 0;
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
