#include <iostream>
#include <random>
#include <cassert>
#include <chrono>
#include "mj_algo1.h"

#define TEST_CALL(f)    do {\
    auto b = std::chrono::system_clock::now();\
    f();\
    auto e = std::chrono::system_clock::now();\
    auto diff  = std::chrono::duration_cast<std::chrono::milliseconds>(e-b).count();\
    cout << "calc "<< #f << " cost:" << diff << "ms" << endl;\
}while(0)

using namespace std;

void test_canhu_nj() {
    struct cardidxs idxs;
    memset(&idxs, 0, sizeof(idxs));
    // 1W2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T
    idxs_add(&idxs, 1, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, 4, 3);idxs_add(&idxs, 8, 2);
    idxs_add(&idxs, 11, 1);idxs_add(&idxs, 12, 1);idxs_add(&idxs, 13, 1);
    idxs_add(&idxs, 16, 1);idxs_add(&idxs, 17, 1);idxs_add(&idxs, 18, 1);
    assert(canhu_nojoker(&idxs));

    memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T7T,9T9T
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 2);
    idxs_add(&idxs, 4, 2);idxs_add(&idxs, 17, 4);idxs_add(&idxs, 19, 2);
    assert(canhu_nojoker(&idxs));
}

void test_travel_allhu() {

}

int main(int argc, char** argv)
{
    TEST_CALL(test_canhu_nj);
    TEST_CALL(test_travel_allhu);
    return 0;
}
