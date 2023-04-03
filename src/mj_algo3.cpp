#include "mj_algo3.h"
#include "mj_algo3_detail.h"
#include "test.h"



static void test_hu_units() {
    hu_card_units hcu;
    zero_struct(hcu);
    auto* item = hcu.M.grap();
    hui_init(item, {1,2,3}, UIT_M, M_ZHONG);
    item = hcu.M.grap();
    hui_init(item, {4,4,4}, UIT_M, M_KEZI);
    print_hui_array(&hcu.M, std::cout);
}

void mj_algo3_test() {
    TEST_CALL(test_hu_units);
}
