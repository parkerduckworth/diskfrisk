/* 
    Compile Tests:
        cc input_test.c -o test_execs/input_test -lcunit

    Execute Tests:
        test_execs/input_test
*/

#include <CUnit/Basic.h>
#include "../src/diskfrisk.c"
#include "../src/prototypes.h"
#include "../src/sysdep.h"
#include <stdio.h>

extern int test;

int init_inputtest_suite(void)
{
    test = 1;
    return 0;
}


int clean_inputtest_suite(void)
{
    test = 0;
    return 0;
}


void flag_test1(void)
{
    // Test for valid input flags
    input(3, (char *[]) {NULL, "-oC", "TEST"});
    CU_ASSERT_EQUAL(option.openf, 1);
    CU_ASSERT_EQUAL(option.csens, 1);
    CU_ASSERT_EQUAL(dname, ROOT);
}


void flag_test2(void)
{
    // Test for valid input flags
    input(3, (char *[]) {NULL, "-h", "TEST"});
    CU_ASSERT_EQUAL(option.home, 1);
    CU_ASSERT_EQUAL(dname, HOME);
}


void flag_test3(void)
{
    // Test for valid input flags
    input(3, (char *[]) {NULL, "-p", "grep:TEST"});
    CU_ASSERT_EQUAL(option.perm, 1);
    CU_ASSERT_EQUAL(option.grep, 1);
}


int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("input_test_suite", init_inputtest_suite, clean_inputtest_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "input() flag test 1", flag_test1) ||
        (NULL == CU_add_test(pSuite, "input() flag test 2", flag_test2)) ||
        (NULL == CU_add_test(pSuite, "input() flag test 3", flag_test3)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
