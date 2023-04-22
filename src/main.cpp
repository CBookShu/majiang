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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

template <typename F>
static bool random_test_cards(F fuc) {
    #define MAX_COUNT (100 * 10000)
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

TEST_SUITE_BEGIN("main_test");

TEST_CASE("check_1_2") {
    gen_cache();
    CHECK_FALSE(random_test_cards([](cardidxs* c){
        auto b1 = canhu(c);
        auto b2 = canhu_2(c);
        if(b1 != b2) {
            print_cardidx(c, std::cout);
            std::cout << b1 << b2 << std::endl;
            return false;
        }
        return true;
    }));
}

TEST_CASE("check_1_3") {
    CHECK_FALSE(random_test_cards([](cardidxs* c){
        auto b1 = canhu(c);
        auto b2 = canhu_3(c);
        if(b1 != b2) {
            print_cardidx(c, std::cout);
            std::cout << b1 << b2 << std::endl;
            return false;
        }
        return true;
    }));
}

TEST_CASE("benchmark_canhu") {
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

TEST_CASE("benchmark_travelhu") {
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

TEST_SUITE_END;

// xmake run majiang --test-suite=MAKE_MDJP_COMPILE
TEST_SUITE_BEGIN("MAKE_MDJP_COMPILE");
// 将MJDP的代码再优化一下，原来的全部铺开写太反人类了！！！


struct MjMJDPCal {
    static bool check_hu(int m,int d,int j,int p,int jk) {
        REQUIRE_EQ(m * 3 + d * 2 + j * 2 + p + jk, HAND_CARDS_COUNT);
        if (d > jk) {
            return false;
        }
        m += d;jk -= d;
        if (p > jk) {
            return false;
        }
        // p 全部组成j
        j += p;jk -= p;
        
        // j只需要1个,分析当前j和jk的剩余情况
        // 先保证有一个j
        int lack_j = 1 - j;
        if (lack_j > 0) {
            // 其实只有1 的情况
            jk -= 2 * lack_j;
            j += lack_j;
        } else if(lack_j < 0) {
            // j过多,全给它组成M
            jk -= (-lack_j);
            m += (-lack_j);
            j -= (-lack_j);
        }
        if(jk < 0) {
            return false;
        }
        if(jk > 0) {
            REQUIRE_EQ(jk % 3, 0);
            m += jk / 3;
            jk = 0;
        }
        return m == 4 && j == 1 && d == 0 && p == 0;
    }
    static void travel_all_hu(int joker, hand_card_units* p, bool(*func)(hu_card_units* u)) {
        hu_card_units u;
        #define QCALL()    if(func(&u)) {return;}

        
    }
};

TEST_CASE("make_mdjp") {
    constexpr int JK = 4;
    constexpr int M = 3;
    constexpr int D = 0;
    constexpr int J = 0;
    constexpr int P = 1;
    

}


TEST_SUITE_END();