#include <initializer_list>
#include <iostream>
#include <random>
#include <memory>
#include <algorithm>
#include <map>
#include "test.h"
#include "mj_algo1.h"
#include "mj_algo2.h"
#include "mj_algo3.h"

using namespace std;

static bool check_algo_with_algo1(cardidxs* ve, bool (canhu_f)(cardidxs *c)) {
    #define MAX_COUNT (100 * 10000)
    std::unique_ptr<cardidxs[]> v = std::make_unique<cardidxs[]>(MAX_COUNT);
    zero_struct(v.get());
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

    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };

    std::cout << "start" << std::endl;
    int hu = 0;
    auto now = std::chrono::system_clock::now();
    for(int i = 0; i < MAX_COUNT; ++i) {
        if(canhu(&v[i]) != canhu_f(&v[i])) {
            print_cardidx(&v[i], std::cout);
            *ve = v[i];
            return true;
        }
        // travel_all_hu(&v[i], f);
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

static void check_algo1_algo2() {
    gen_cache();
    cardidxs v;
    zero_struct(v);
    if(check_algo_with_algo1(&v, canhu_2)) {
        test_1(v);
    }
}

static void test_2(cardidxs v) {
    print_cardidx(&v, std::cout);
    auto b1 = canhu(&v);
    auto b2 = canhu_3(&v);

    std::cout << b1 << b2 << std::endl;
}

// TODO: 验证canhu_3 的正确性
static void check_algo1_algo3() {
    gen_cache();
    cardidxs v;
    zero_struct(v);
    if(check_algo_with_algo1(&v, canhu_3)) {
        test_2(v);
    }
}

int main(int argc, char** argv)
{
    // mj_algo3_test();
    check_algo1_algo3();
    return 0;
}
