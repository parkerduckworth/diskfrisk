/* 
    Compile Tests:
        cc test_frisk.c -o test_execs/test_frisk -lcunit

    Execute Tests:
        test_execs/test_frisk
*/

#include <CUnit/Basic.h>
#include "../src/diskfrisk.c"
#include "../src/prototypes.h"
#include "../src/sysdep.h"
#include <stdio.h>

#define TFNAME "TEST"  // <filename> used for testing

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


void resline_test(void)
{
    frisk("\0", HOME);
    CU_ASSERT_EQUAL(found, 0);
}


int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("frisk() Test Suite", init_inputtest_suite, clean_inputtest_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Result Line Test", resline_test))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
