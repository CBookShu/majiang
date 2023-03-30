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
    n = n << ((9) * VALUE_BITS);
    k |= n;
    return k;
}
static char cardskey_2values(cards_key k,cardvalues*c) {
    char res = 0;
    for(int i = 0; i < 9; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        res += n;
        if(c) {
            c->idxs[i] = n;
            c->count += n;
        }
    }
    auto n = (k >> ((9) * VALUE_BITS)) & VALUE_MASK_JOKER;
    res += n;
    if(c) {
        c->idxs[9] = n;
        c->count += n;
    }
    return res;
}
static cards_key idx2_cardskey(cardidxs* c, int shape, int* cards_count) {
    cards_key k = 0;
    int count = shape == SHAPE_FENG ? 7 : 9;
    for(int i = 0; i < count; ++i) {
        auto n = VALUE_MASK & c->idxs[shape * 9 + i];
        if(cards_count) {
            *cards_count += n;
        }
        n = n << ((i) * VALUE_BITS);
        k |= n;
    }
    auto n = VALUE_MASK_JOKER & c->idxs[JOKER_INDEX];
    if(cards_count) {
        *cards_count += n;
    }
    n = n << ((9) * VALUE_BITS);
    k |= n;
    return k;
}
static void cardskey_2idx(cards_key k, int shape, cardidxs* c) {
    int count = shape == SHAPE_FENG ? 7 : 9;
    for(int i = 0; i < count; ++i) {
        auto n = (k >> ((i) * VALUE_BITS)) & VALUE_MASK;
        c->idxs[shape*9 + i] = n;
        c->count += n;
    }
    auto n = (k >> ((9) * VALUE_BITS)) & VALUE_MASK_JOKER;
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
