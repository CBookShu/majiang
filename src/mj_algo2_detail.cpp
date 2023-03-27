#include "mj_algo2_detail.h"
#include <unordered_set>
#include <functional>

using namespace mj_algo2;
using mj_key_cache_t = std::unordered_set<cards_key>;
static mj_key_cache_t g_cache_keys_wtd[JOKER_MAX+1];
static mj_key_cache_t g_cache_keys_fb[JOKER_MAX+1];
static mj_key_cache_t g_cache_keys_7j[JOKER_MAX+1];

static void _gen_cache_with_joker(cardvalues* v, mj_key_cache_t cache[]) {
    for(int i = 0; i < 9; ++i) {
        if(v->idxs[i] > 4) {
            return;
        }
    }
    print_cardvalues(v, std::cout);
    auto k = values2_cardskey(v);
    int jokercount = v->idxs[9];
    if(jokercount > JOKER_MAX) {
        return;
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
    if(level > 4) {
        return ;
    }
    _gen_cache_with_joker(v, fb ? g_cache_keys_fb : g_cache_keys_wtd);
    for (int i = 0; i < 9; ++i) {
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

static void _gen_cache_7j(cardvalues* v,char level) {
    // TODO: 完成7对的缓存
}

void mj_algo2::gen_cache()
{
    cardvalues values;
    std::memset(&values,0,sizeof(values));
    bool jiang = false;
    char level = 1;
    _gen_cache_4m_1j(&values, jiang, level, false);
    _gen_cache_4m_1j(&values, jiang, level, true);


    std::size_t sz = 0;
    for(int i = 0; i < std::size(g_cache_keys_wtd); ++i) {
        sz += g_cache_keys_wtd[i].size();
    }
    std::cout << "4m1j total cache: "<< sz << std::endl;
}
