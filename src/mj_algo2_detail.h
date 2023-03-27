#pragma once
#include "utils.h"
#include <cstdint>

#define VALUE_BITS      3
#define VALUE_MASK      0x07

typedef std::uint32_t    cards_key;
typedef struct cardvalues {
    char idxs[9];
    char count;
}cardvalues;


static void values_add(cardvalues* c, char idx, char add) {
    (c)->idxs[idx]+=add;(c)->count+=add;
}
static cards_key values2_cardskey(cardvalues*c) {
    cards_key k = 0;
    for(int i = 0; i < 9; ++i) {
        auto n = VALUE_MASK & c->idxs[i];
        n = n << ((i - 1) * VALUE_BITS);
        k |= n;
    }
    return k;
}
static void cardskey_2values(cards_key k,cardvalues*c) {
    for(int i = 0; i < 9; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[i] = n;
        c->count += n;
    }
}
static cards_key idx2_cardskey(cardidxs* c, int shape) {
    cards_key k = 0;
    for(int i = 0; i < 9; ++i) {
        auto n = VALUE_MASK & c->idxs[shape * 9 + i];
        n = n << ((i) * VALUE_BITS);
        k |= n;
    }
    return k;
}
static void cardskey_2idx(cards_key k, int shape, cardidxs* c) {
    for(int i = 0; i < 9; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[shape*9 + i] = n;
        c->count += n;
    }
}

namespace mj_algo2 {
    void gen_cache();
    

};