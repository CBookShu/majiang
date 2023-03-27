#include "mj_algo2.h"
#include "test.h"
#include <sstream>

static void test_idxs_convert_cardskey() {
    cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));

    // 1W,1W,2W,2W,3W,3W,4W,4W,5W,6W,7W
    idxs_add(&idxs, W(1), 2);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 2);
    idxs_add(&idxs, W(4), 2);idxs_add(&idxs, W(5), 1);idxs_add(&idxs, W(6), 1);
    idxs_add(&idxs, W(7), 1);

    std::stringstream ss[2];
    auto k = idx2_cardskey(&idxs, SHAPE_WAN);
    print_cardidx(&idxs, ss[0]);
    print_cardidx(&idxs, std::cout);
    std::cout << k << std::endl;

    std::memset(&idxs, 0, sizeof(idxs));
    cardskey_2idx(k, SHAPE_WAN, &idxs);
    print_cardidx(&idxs, ss[1]);
    print_cardidx(&idxs, std::cout);
    assert(ss[0].str() == ss[1].str());
}

void test_algo2()
{
    TEST_CALL(test_idxs_convert_cardskey);
}
