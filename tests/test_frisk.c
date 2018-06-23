/* 
    Compile Tests:
        cc test_frisk.c -o test_execs/test_frisk -lcunit

    Execute Tests:
        test_execs/test_frisk
*/

#include <CUnit/Basic.h>
#include "../src/diskfrisk.c"
#include "../src/display.c"
#include "../src/extern.h"
#include "../src/prototypes.h"
#include "../src/sysdep.h"
#include <stdio.h>
#include <time.h>

#define TFNAME "TEST"  // <filename> used for testing

extern int test;
extern int found;
clock_t t_start, t_end;
double t_t_elapsed;

int init_frisktest_suite(void)
{
    test = 1;
    return 0;
}


int clean_frisktest_suite(void)
{
    test = 0;
    return 0;
}


void t_elapsed_test(void)
{
    t_start = clock();
    frisk("", HOME);
    t_end = clock();
    t_t_elapsed = ((double)(t_end-t_start)) / CLOCKS_PER_SEC;

    CU_ASSERT_TRUE(t_t_elapsed < t_elapsed + .00001 && t_t_elapsed > t_elapsed - .00001);
}


int main() 
{   
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("frisk() Test Suite", init_frisktest_suite, clean_frisktest_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Time Elapsed Test", t_elapsed_test))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
   
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
