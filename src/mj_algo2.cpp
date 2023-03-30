#include "mj_algo2.h"
#include "test.h"
#include <sstream>
#include <unordered_set>
#include <functional>
#include <memory>
#include <set>

using mj_key_cache_t = std::unordered_set<cards_key>;
static mj_key_cache_t g_cache_keys_4m1j_nf[JOKER_MAX+1];
static mj_key_cache_t g_cache_keys_4m1j_f[JOKER_MAX+1];
static mj_key_cache_t g_cache_keys_7j[JOKER_MAX+1];

static std::set<cards_key> bad_route[3][JOKER_MAX + 1];

static bool check_add(cardvalues_1* v, int joker, bool feng, bool j7) {
    auto k = values2_cardskey(v);
    if (k == 0) return false;

    mj_key_cache_t* cache;
    if(j7) {
        cache = &(g_cache_keys_7j[joker]);
        auto& br = bad_route[0][joker];
        if(br.count(k)) {
            return false;
        }
        br.insert(k);
    } else {
        if(feng) {
            cache = &(g_cache_keys_4m1j_f[joker]);
            auto& br = bad_route[1][joker];
            if(br.count(k)) {
                return false;
            }
            br.insert(k);
        } else {
            cache = &(g_cache_keys_4m1j_nf[joker]);
            auto& br = bad_route[2][joker];
            if(br.count(k)) {
                return false;
            }
            br.insert(k);
        }
    }

    auto it = cache->find(k);
    if(it != cache->end()) {
        return false;
    }

    for(int i = 0; i < 9; ++i) {
        if(v->idxs[i] > 4) {
            return true;
        }
    }
    cache->insert(k);
    return true;
}

static void parse_table_sub(cardvalues_1* v, int joker, bool feng, bool j7) {
    int count = feng ? 7 : 9;
    for(int i = 0; i < count; ++i) {
        if(v->idxs[i] == 0) continue;

        values_add(v, i, -1);
        if(!check_add(v, joker, feng, j7)) {
            values_add(v, i, 1);
            continue;
        }

        if(joker < JOKER_MAX) {
            parse_table_sub(v, joker + 1, feng, j7);
        }

        values_add(v, i, 1);
    }
}

static void parse_table(cardvalues_1* v, bool feng, bool j7) {
    if(!check_add(v, 0, feng, j7)) {
        return;
    }
    parse_table_sub(v, 1, feng, j7);
}

static void gen_auto_table_sub(cardvalues_1* v, int level, bool feng) {
    int count = feng ? 7 : 16;
    for(int i = 0; i < count; ++i) {
        if(i <= 8) {
            if(v->idxs[i] > 3) continue;
            values_add(v, i, 3);
        } else {
            int index = i - 9;
            if(v->idxs[index] > 5 || v->idxs[index + 1] > 5 || v->idxs[index + 2] > 5) continue;
            values_add(v, index, 1);
            values_add(v, index+1, 1);
            values_add(v, index+2, 1);
        }
        parse_table(v, feng, false);
        if(level < 4) {
            gen_auto_table_sub(v, level+1, feng);
        }
        if(i <= 8) {
            values_add(v, i, -3);
        } else {
            int index = i - 9;
            values_add(v, index, -1);
            values_add(v, index+1, -1);
            values_add(v, index+2, -1);
        }
    }
}

static void gen_4m1j_table() {
    cardvalues_1 cards;
    memset(&cards, 0, sizeof(cards));
    gen_auto_table_sub(&cards, 1, false);
    gen_auto_table_sub(&cards, 1, true);
    
    for(int i = 0; i < 9; ++i) {
        values_add(&cards, i, 2);
        gen_auto_table_sub(&cards, 1, false);
        parse_table(&cards, false, false);
        if(i < 7) {
            parse_table(&cards, true, false);
            gen_auto_table_sub(&cards, 1, true);
        }
        values_add(&cards, i, -2);
    }
}

static void gen_auto_7dui_table(cardvalues_1* v, int level) {
    for(int i = 0; i < 9; ++i) {
        if(v->idxs[i] >= 4) continue;

        values_add(v, i, 2);
        parse_table(v, false, true);
        if (level < 7) {
            gen_auto_7dui_table(v, level + 1);
        }
        values_add(v, i, -2);
    }
}

static void gen_7j_table() {
    cardvalues_1 cards;
    memset(&cards, 0, sizeof(cards));
    gen_auto_7dui_table(&cards, 1);
}

void gen_cache()
{
    gen_4m1j_table();
    gen_7j_table();

    std::size_t sz = 0;
    std::size_t sz1 = 0;
    std::size_t sz2 = 0;
    for(int i = 0; i < std::size(g_cache_keys_4m1j_nf); ++i) {
        sz += g_cache_keys_4m1j_nf[i].size();
        sz1 += g_cache_keys_4m1j_f[i].size();
        sz2 += g_cache_keys_7j[i].size();
    }
    std::cout << "total cache nofb: "<< sz << std::endl;
    std::cout << "total cache fb: "<< sz1 << std::endl;
    std::cout << "total cache 7d: "<< sz2 << std::endl;
}

static bool _canhu_7j_2(cardidxs* c) {
    int joker = c->idxs[JOKER_INDEX];
    for(int i = 0; i < SHAPE_FENG+1;++i) {
        auto& cache =g_cache_keys_7j;
        int cards_count = 0;
        auto k = idx2_cardskey(c, i, &cards_count);
        if(k == 0) continue;

        bool ok = false;
        for(int j = 0; j <= joker; ++j) {
            if(cache[j].count(k)) {
                ok = true;
                joker -= j;
                break;
            } 
        }
        if(!ok) {
            return false;
        }
        if(joker < 0) {
            return false;
        }
    }
    return true;
}

static bool _canhu_4m1j_2(cardidxs* c) {
    int joker = c->idxs[JOKER_INDEX];
    int jiang = 0;
    for(int i = 0; i < SHAPE_FENG+1;++i) {
        auto& cache = (i==SHAPE_FENG)?g_cache_keys_4m1j_f:g_cache_keys_4m1j_nf;
        int cards_count = 0;
        auto k = idx2_cardskey(c, i, &cards_count);
        if(k == 0) continue;

        bool ok = false;
        for(int j = 0; j <= joker; ++j) {
            auto yu = (cards_count+j) % 3;
            if (yu == 1) continue;
            if(cache[j].count(k)) {
                ok = true;
                joker -= j;
                if(yu == 2) {
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

bool canhu_2(cardidxs *c)
{
    // 首先要保证c 中的牌张数量和格式一定要正确
    // 分别对万，条，筒进行判断
    return _canhu_7j_2(c) || _canhu_4m1j_2(c);
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

static void test_rnd_canhu_2() {
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
    // TEST_CALL(test_idxs_convert_cardskey);

    // TEST_CALL(test_gen_cache);

    // TEST_CALL(test_can_hu);

    TEST_CALL(test_gen_cache);
    // TEST_CALL(test_rnd_canhu_2);
}
