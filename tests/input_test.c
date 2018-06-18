// cc input_test.c -o test_execs/input_test -lcunit

#include <CUnit/Basic.h>
#include "../src/prototypes.h"
#include "../src/sysdep.h"
#include "../src/diskfrisk.c"
#include <stdio.h>


void flag_test1(void)
{
    extern int test;

    test = 1;
    // Test for valid input flags
    input(3, (char*[]) {NULL, "-oC", "TEST"});
    CU_ASSERT_EQUAL(option.openf, 1);
    CU_ASSERT_EQUAL(option.csens, 1);
    CU_ASSERT_EQUAL(dname, ROOT);
    test = 0;
}


void flag_test2(void)
{
    extern int test;

    test = 1;
    // Test for valid input flags
    input(3, (char*[]) {NULL, "-h", "TEST"});
    CU_ASSERT_EQUAL(option.home, 1);
    CU_ASSERT_EQUAL(dname, HOME);
    test = 0;
}


int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("input_test_suite", 0, 0);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "input() flag test 1", flag_test1) ||
        (NULL == CU_add_test(pSuite, "input() flag test 2", flag_test2)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
