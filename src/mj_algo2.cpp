#include "mj_algo2.h"
#include "test.h"
#include <sstream>
#include <unordered_set>
#include <functional>
#include <memory>

using mj_key_cache_t = std::unordered_set<cards_key>;
static mj_key_cache_t g_cache_keys_wtd[JOKER_MAX+1];
static mj_key_cache_t g_cache_keys_fb[JOKER_MAX+1];

static void _gen_cache_with_joker(cardvalues* v, mj_key_cache_t cache[]) {
    // print_cardvalues(v, std::cout);
    int jokercount = v->idxs[9];
    v->idxs[9] = 0;
    auto k = values2_cardskey(v);
    v->idxs[9] = jokercount;
    if(jokercount > JOKER_MAX) {
        return;
    }
    if(k == 0) {
        return ;
    }
    auto it = cache[jokercount].insert(k);
    if(!it.second) {
        // 重复的k
        return;
    }

    for(int i = 0; i < 9; ++i) {
        if(v->idxs[i] > 0) {
            values_add(v, i, -1);
            values_add(v, 9, 1);
            _gen_cache_with_joker(v, cache);
            values_add(v, 9, -1);
            values_add(v, i, 1);
        }
    }
}

static void _gen_cache_4m_1j(cardvalues* v, bool jiang, char level, bool fb) {
    if(level > 5) {
        return ;
    }
    _gen_cache_with_joker(v, fb ? g_cache_keys_fb : g_cache_keys_wtd);
    int count = fb ? 7 : 9;
    for (int i = 0; i < count; ++i) {
        if(v->idxs[i] >= 4) {
            continue;
        }

        // 刻字
        if (v->idxs[i] < 2) {
            values_add(v, i, 3);
            level++;
            _gen_cache_4m_1j(v, jiang, level, fb);
            level--;
            values_add(v, i, -3);
        }
        // 将
        if(!jiang&&v->idxs[i] < 3) {
            jiang = true;
            level++;
            values_add(v, i, 2);
            _gen_cache_4m_1j(v, jiang, level, fb);
            level--;
            values_add(v, i, -2);
            jiang = false;
        }
        // 顺
        if(!fb && i <= 7 && v->idxs[i+1] < 4 && v->idxs[i+2]<4) {
            values_add(v, i, 1);
            values_add(v, i+1, 1);
            values_add(v, i+2, 1);
            level++;
            _gen_cache_4m_1j(v, jiang, level, fb);
            level--;
            values_add(v, i, -1);
            values_add(v, i+1, -1);
            values_add(v, i+2, -1);
        }
    }
}

static void _gen_cache_7j(cardvalues* v, char level, bool fb) {
    if(level > 7) {
        return ;
    }
    _gen_cache_with_joker(v, fb ? g_cache_keys_fb : g_cache_keys_wtd);
    int count = fb ? 7 : 9;
    for (int i = 0; i < count; ++i) {
        if(v->idxs[i] >= 4) {
            continue;
        }
        level++;
        values_add(v, i, 2);
        _gen_cache_7j(v, level, fb);
        level--;
        values_add(v, i, -2);
    }
}

void gen_cache()
{
    cardvalues values;
    std::memset(&values,0,sizeof(values));
    bool jiang = false;
    char level = 0;
    _gen_cache_4m_1j(&values, jiang, level, false);
    _gen_cache_7j(&values, level, false);
    _gen_cache_4m_1j(&values, jiang, level, true);
    _gen_cache_7j(&values, level, true);

    std::size_t sz = 0;
    std::size_t sz1 = 0;
    for(int i = 0; i < std::size(g_cache_keys_wtd); ++i) {
        sz += g_cache_keys_wtd[i].size();
        sz1 += g_cache_keys_fb[i].size();
    }
    std::cout << "total cache nofb: "<< sz << std::endl;
    std::cout << "total cache fb: "<< sz1 << std::endl;
}

bool canhu_2(cardidxs *c)
{
    // 首先要保证c 中的牌张数量和格式一定要正确
    // 分别对万，条，筒进行判断
    int joker = c->idxs[JOKER_INDEX];
    int jiang = 0;
    for(int i = 0; i < SHAPE_FENG+1;++i) {
        auto& cache = (i==SHAPE_FENG)?g_cache_keys_fb:g_cache_keys_wtd;
        int cards_count = 0;
        auto k = idx2_cardskey(c, i, &cards_count);
        if(k == 0) continue;

        bool ok = false;
        for(int j = 0; j <= joker; ++j) {
            if(cache[j].count(k)) {
                ok = true;
                joker -= j;
                if((cards_count+j) % 3 == 2) {
                    jiang++;
                }
                break;
            } 
        }
        if(!ok) {
            return false;
        }
        if(joker < 0) {
            return false;
        }
        if((jiang-1) > joker) {
            return false;
        }
    }
    return true;
}

void travel_all_hu_2(cardidxs *c, bool (*f)(cardsunit *))
{

}

static void test_idxs_convert_cardskey() {
    cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));

    // 1W,1W,2W,2W,3W,3W,4W,4W,5W,6W,7W
    idxs_add(&idxs, W(1), 2);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 2);
    idxs_add(&idxs, W(4), 2);idxs_add(&idxs, W(5), 1);idxs_add(&idxs, W(6), 1);
    idxs_add(&idxs, W(7), 1);

    std::stringstream ss[2];
    auto k = idx2_cardskey(&idxs, SHAPE_WAN, nullptr);
    print_cardidx(&idxs, ss[0]);
    print_cardidx(&idxs, std::cout);
    std::cout << k << std::endl;

    std::memset(&idxs, 0, sizeof(idxs));
    cardskey_2idx(k, SHAPE_WAN, &idxs);
    print_cardidx(&idxs, ss[1]);
    print_cardidx(&idxs, std::cout);
    assert(ss[0].str() == ss[1].str());
}

static void test_gen_cache() {
    gen_cache();
}

static void test_can_hu() {
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T,JOKER
    idxs_add(&idxs, JOKER_INDEX, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, W(4), 3);idxs_add(&idxs, W(8), 2);
    idxs_add(&idxs, T(1), 1);idxs_add(&idxs, T(2), 1);idxs_add(&idxs, T(3), 1);
    idxs_add(&idxs, T(6), 1);idxs_add(&idxs, T(7), 1);idxs_add(&idxs, T(8), 1);
    assert(canhu_2(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T,9T9T,JOKER
    idxs_add(&idxs, W(1), 2);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 2);
    idxs_add(&idxs, T(4), 2);idxs_add(&idxs, T(7), 3);idxs_add(&idxs, T(9), 2);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu_2(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W1W,2W2W,3W3W3W,7W7W,7W8W9W,JOKER
    idxs_add(&idxs, W(1), 3);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 3);
    idxs_add(&idxs, W(7), 3);idxs_add(&idxs, W(8), 1);idxs_add(&idxs, W(9), 1);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu_2(&idxs));
}

static void test_rnd_canhu() {
    #define MAX_COUNT (9 * 100 * 10000)
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
        hu += canhu_2(&v[i]);
    }
    auto after = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(after-now).count();
    std::cout << "查表法总数:" << MAX_COUNT / 10000 << "万次，time:" << diff << "ms" << std::endl;
	std::cout << "Hu: " << hu << std::endl;
}

void test_algo2()
{
    TEST_CALL(test_idxs_convert_cardskey);

    TEST_CALL(test_gen_cache);

    TEST_CALL(test_can_hu);

    TEST_CALL(test_rnd_canhu);
}
