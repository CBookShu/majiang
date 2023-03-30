#include <iostream>
#include <random>
#include <memory>
#include <algorithm>
#include "test.h"
#include "mj_algo1.h"
#include "mj_algo2.h"

using namespace std;

static bool check_algo1_with_algo2(cardidxs* ve) {
    #define MAX_COUNT (100 * 10000)
    std::unique_ptr<cardidxs[]> v = std::make_unique<cardidxs[]>(MAX_COUNT);
    std::memset(v.get(), 0, sizeof(v));
    srand(1);
    char cards[TOAL_CARDS_NOJOKER];
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        memset(cards+i*4, i, 4);
    }
    int joker_count = JOKER_MAX;
    for(int i = 0; i < MAX_COUNT; ++i) {
        std::random_shuffle(std::begin(cards), std::end(cards));
        v[i].count = HAND_CARDS_COUNT;
        for(int j = 0; j < HAND_CARDS_COUNT - joker_count;++j) {
            v[i].idxs[cards[j]]++;
        }
        v[i].idxs[JOKER_INDEX] = joker_count;
    }

    std::cout << "start" << std::endl;
    int hu = 0;
    auto now = std::chrono::system_clock::now();
    for(int i = 0; i < MAX_COUNT; ++i) {
        // if(canhu(&v[i]) != canhu_2(&v[i])) {
        //     print_cardidx(&v[i], std::cout);
        //     *ve = v[i];
        //     return true;
        // }
        canhu_2(&v[i]);
    }
    std::cout << "end" << std::endl;
    auto after = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(after-now).count();
    std::cout << "回溯法:" << MAX_COUNT / 10000 << "万次，time:" << diff << "ms" << std::endl;
	std::cout << "Hu: " << hu << std::endl;
    return false;
}

static void test_1(cardidxs v) {
    print_cardidx(&v, std::cout);
    auto b1 = canhu(&v);
    auto b2 = canhu_2(&v);

    std::cout << b1 << b2 << std::endl;
}

int main(int argc, char** argv)
{
    // test_algo1();
    test_algo2();

    // gen_cache();
    
    cardidxs v;
    if(check_algo1_with_algo2(&v)) {
        test_1(v);
    }

    return 0;
}
