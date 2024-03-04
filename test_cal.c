//
// Created by 윤일준 on 2024/03/04.
//

#include <CUnit/Basic.h>
#include "cal.h"

void test_add(void) {
    CU_ASSERT(add(0, 0) == 0);
    CU_ASSERT(add(2, 2) == 4);
    CU_ASSERT(add(-1, 1) == 0);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Suite_1", 0, 0);

    CU_add_test(suite, "test of add()", test_add);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
