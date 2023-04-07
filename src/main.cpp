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
#include "utils.h"

using namespace std;

template <typename F>
static bool random_test_cards(F fuc) {
    #define MAX_COUNT (800 * 10000)
    std::unique_ptr<cardidxs[]> v = std::make_unique<cardidxs[]>(MAX_COUNT);
    zero_struct(v.get());
    srand(1);
    char cards[TOAL_CARDS_NOJOKER];
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        memset(cards+i*4, i, 4);
    }
    for(int i = 0; i < MAX_COUNT; ++i) {
        std::random_shuffle(std::begin(cards), std::end(cards));
        int joker_count = rand() % (JOKER_MAX + 1);
        v[i].count = HAND_CARDS_COUNT;
        for(int j = 0; j < HAND_CARDS_COUNT - joker_count;++j) {
            v[i].idxs[cards[j]]++;
        }
        v[i].idxs[JOKER_INDEX] = joker_count;
    }

    std::cout << "start" << std::endl;
    auto now = std::chrono::system_clock::now();
    int i = 0;
    for(i = 0; i < MAX_COUNT; ++i) {
        if(!fuc(&v[i])) {
            return true;
        }
    }
    std::cout << "end" << std::endl;
    auto after = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(after-now).count();
    std::cout << "" << i << "次,time:" << diff << "ms" << std::endl;
    return false;
}

static void check_algo1_algo2() {
    gen_cache();
    random_test_cards([](cardidxs* c){
        auto b1 = canhu(c);
        auto b2 = canhu_2(c);
        if(b1 != b2) {
            print_cardidx(c, std::cout);
            std::cout << b1 << b2 << std::endl;
            return false;
        }
        return true;
    });
}

static void test_2(cardidxs v) {
    print_cardidx(&v, std::cout);
    auto b1 = canhu(&v);
    auto b2 = canhu_3(&v);

    std::cout << b1 << b2 << std::endl;
}

// TODO: 验证canhu_3 的正确性
static void check_algo1_algo3() {
    random_test_cards([](cardidxs* c){
        auto b1 = canhu(c);
        auto b2 = canhu_3(c);
        if(b1 != b2) {
            print_cardidx(c, std::cout);
            std::cout << b1 << b2 << std::endl;
            return false;
        }
        return true;
    });
}

static void benchmark_canhu() {
    TEST_CALL([](){
        random_test_cards([](cardidxs* c){
            canhu(c);
            return true;
        });
    });

    TEST_CALL([](){
        random_test_cards([](cardidxs* c){
            canhu_2(c);
            return true;
        });
    });

    TEST_CALL([](){
        random_test_cards([](cardidxs* c){
            canhu_3(c);
            return true;
        });
    });
}

static void benchmark_travelhu() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        // print_cardsunit(u, std::cout);
        return false;
    };
    bool(*f1) (hu_card_units*) = nullptr;
    f1 = [](hu_card_units* u){
        return false;
    };

    TEST_CALL([&](){
        random_test_cards([&](cardidxs* u){
            travel_all_hu(u, f);
            return true;
        });
    });

    // TEST_CALL([](){
    //     cardidxs v;
    //     zero_struct(v);
    //     check_algo_with_algo1(&v, canhu_2);
    // });

    TEST_CALL([&](){
        random_test_cards([&](cardidxs* u){
            travel_all_hu_3(u, f1);
            return true;
        });
    });
}


int main(int argc, char** argv)
{
    // check_algo1_algo2();
    // check_algo1_algo3();
    
    // benchmark_canhu();
    return 0;
}
