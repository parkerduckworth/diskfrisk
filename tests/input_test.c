// gcc input_test.c -o input_test -lcunit

#include <CUnit/Basic.h>

int main() {
   // Initialize the CUnit test registry
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();
   // Sets the basic run mode, CU_BRM_VERBOSE will show maximum output of run details
   // Other choices are: CU_BRM_SILENT and CU_BRM_NORMAL
   CU_basic_set_mode(CU_BRM_VERBOSE);
   // Run the tests and show the run summary
   CU_basic_run_tests();
   return CU_get_error();
}