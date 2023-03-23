#pragma once
#include "utils.h"
#include <cstdint>

#define VALUE_BITS      3
#define VALUE_MASK      0x07

typedef std::int32_t    cards_key;


static cards_key idx2_cardskey(cardidxs* c, int shape) {
    cards_key k = 0;
    for(int i = 1; i <= 9; ++i) {
        auto n = VALUE_MASK & c->idxs[shape * 10 + i];
        n = n << ((i - 1) * VALUE_BITS);
        k |= n;
    }
    return k;
}
static void cardskey_2idx(cards_key k, int shape, cardidxs* c) {
    for(int i = 1; i <= 9; ++i) {
        auto n = (k >> ((i - 1) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[shape*10 + i] = n;
        c->count += n;
    }
}

struct MJCacheConfigDefault {
    static const int MJ_HAND_CARDS_COUNT = 14;      //14 张牌
    static const int MJ_LAYOUT_NUM = 34;            //
};