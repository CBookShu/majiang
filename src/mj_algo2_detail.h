#pragma once
#include "utils.h"
#include <cstdint>

#define VALUE_BITS          3
#define VALUE_MASK          0x07
#define VALUE_BITS_JOKER    5
#define VALUE_MASK_JOKER    0x20

typedef std::uint32_t    cards_key;
typedef struct cardvalues {
    char idxs[10];
    char count;
}cardvalues;


static void values_add(cardvalues* c, char idx, char add) {
    (c)->idxs[idx]+=add;(c)->count+=add;
}
static cards_key values2_cardskey(cardvalues*c) {
    cards_key k = 0;
    for(int i = 0; i < 9; ++i) {
        auto n = VALUE_MASK & c->idxs[i];
        n = n << ((i) * VALUE_BITS);
        k |= n;
    }
    auto n = VALUE_MASK_JOKER & c->idxs[9];
    n = n << ((10) * VALUE_BITS_JOKER);
    k |= n;
    return k;
}
static void cardskey_2values(cards_key k,cardvalues*c) {
    for(int i = 0; i < 9; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[i] = n;
        c->count += n;
    }
    auto n = (k >> ((10) * VALUE_BITS_JOKER)) & VALUE_MASK_JOKER;
    c->idxs[9] = n;
    c->count += n;
}
static cards_key idx2_cardskey(cardidxs* c, int shape) {
    cards_key k = 0;
    for(int i = 0; i < 9; ++i) {
        auto n = VALUE_MASK & c->idxs[shape * 9 + i];
        n = n << ((i) * VALUE_BITS);
        k |= n;
    }
    auto n = VALUE_MASK_JOKER & c->idxs[JOKER_INDEX];
    n = n << ((10) * VALUE_BITS_JOKER);
    k |= n;
    return k;
}
static void cardskey_2idx(cards_key k, int shape, cardidxs* c) {
    for(int i = 0; i < 9; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[shape*9 + i] = n;
        c->count += n;
    }
    auto n = (k >> ((10) * VALUE_BITS_JOKER)) & VALUE_MASK_JOKER;
    c->idxs[JOKER_INDEX] = n;
    c->count += n;
}

static void print_cardvalues(cardvalues* v, std::ostream& os) {
    for(int i = 0; i < std::size(v->idxs); ++i) {
        os << (int)v->idxs[i];
        os << ",";
    }
    os << std::endl;
}

namespace mj_algo2 {
    void gen_cache();
    

};