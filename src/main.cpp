#include <iostream>
#include <random>
#include "utils.h"

using namespace std;
int main(int argc, char** argv)
{
    default_random_engine e{random_device{}()};
    uniform_int_distribution<int> u(0, 113);

    for(int i = 0; i < 10000; ++i) {
        int id = u(e);
        int shape = get_card_shape(id);
        int value = get_card_value(id);
        int idx = get_card_idx(id);
        printf("cardid[%d] cardshape[%d,%s],cardvalue[%d],cardidx[%d],cardname:%s\r\n",
        id, shape, get_shape_name(shape), value, idx, get_card_name(idx));
    }
    return 0;
}
