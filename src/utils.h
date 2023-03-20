#pragma once
#include <iostream>
#include <cstring>
#include <array>
#include <cassert>

/*
    114 cardid:
    id: 0~35 万:
    0~8     1万~9万
    9~17    1万~9万
    18~26    1万~9万
    27~35    1万~9万
    id: 36~71 条:
    36~44   1条~9条
    ...
    id: 72~107 筒:
    72~81     1筒~9筒
    ...
    108~113    财神

    36 cardidx:
    1 ~ 9:          1万~9万
    11 ~ 19:        1条~9条
    21 ~ 29:        1筒~9筒
    35:             6个财神一样
*/

#define TOAL_CARDS                  114
#define HAND_CARDS_COUNT            14
#define HAND_CARDIDX_LAY            36
#define HAND_CARDIDX_LAY_NOJOKER    30
#define TOTAL_CARD_VALUE            28// 1W~9W,1T~9T,1D~9D,JOKER
#define UNIT_CARDS_MAX_COUNT        4
#define UNIT_MAX_COUNT              7

#define INVALID_ID                  (std::numeric_limits<int>::min())
#define SHAPE_WAN                   0
#define SHAPE_TIAO                  1
#define SHAPE_DONG                  2
#define SHAPE_JOKER                 3
#define ONE_COLOR_COUNT             36
#define JOKER_INDEX                 35         

// 获取牌id的花色
int get_card_shape(int id);

int get_card_shape_byidx(int idx);

// 获取牌面值
int get_card_value(int id);

int get_card_value_byidx(int idx);

// 获取牌idx
int get_card_idx(int id);

// 获取牌的名字
const char* get_shape_name(int shape);

const char* get_card_name(int idx);


typedef struct cardids {
    int ids[HAND_CARDS_COUNT];
    int count;
}cardids;

typedef struct cardidxs {
    int idxs[HAND_CARDIDX_LAY];
    int count;
}cardidxs;

typedef struct cardsunititem {
    int ids[UNIT_CARDS_MAX_COUNT];
    int count;
#define UNIT_ITEM_UNKNOWN_T     0
// 刻字 三张一样的 1W1W1W 
#define UNIT_ITEM_KEZI_T        1
// 顺 三张相连 1W2W3W
#define UNIT_ITEM_SHUN_T        2
// 将 2张相同 1W1W
#define UNIT_ITEM_JIANG_T       3
// 连 2张相连 1W2W
#define UNIT_ITEM_LIAN_T        4
// 坎 2张相隔 1W3W
#define UNIT_ITEM_KAN_T         6
// 单 1张牌
#define UNIT_ITEM_DAN_T         7
    int type;
}cardsunititem;

typedef struct cardsunit {
    struct cardsunititem units[UNIT_MAX_COUNT];
    int count;
}cardsunit;

static void ids_init(cardids* c) {
    c->count = 0;
    for (auto& n : c->ids) {
        n = INVALID_ID;
    }
}

static void ids_add(cardids* c, int id) {
    c->ids[c->count++] = id;
}

static void ids2idxs(cardids* c1, cardidxs* c2) {
    std::memset(c2, 0, sizeof(*c2));
    for (auto n : c1->ids) {
        if (n != INVALID_ID) {
            c2->idxs[get_card_idx(n)]++;
            c2->count++;
        }
    }
}

static void idxs_add(cardidxs* c, int idx, int add) {
    (c)->idxs[idx]+=add;(c)->count+=add;
}

template <typename...Args>
static void init_unititem(cardsunititem* u, int t, Args&&... is) {
    u->type = t;
    int i = 0;
    auto f = [&](auto v){
        u->ids[i++] = v;
    };
    (..., f(is));
    u->count = i;
    for (; i < std::size(u->ids); ++i) {
        u->ids[i] = INVALID_ID;
    }
}

static void init_cardsunit(cardsunit* us) {
    (us)->count=0;
    for(auto&u:(us)->units){init_unititem(&u,UNIT_ITEM_UNKNOWN_T);}
}

static auto grab_cardsunit_item(cardsunit* us)-> cardsunititem* {
    assert(us->count != std::size(us->units));
    return &us->units[us->count++];
}

static void pop_cardsunit_item(cardsunit* us) {
    us->units[us->count--].count = 0;
}

static void print_cardidx(cardidxs* c, std::ostream& os) {
    for(int i = 0; i < HAND_CARDIDX_LAY; ++i) {
        for(int j = 0; j < c->idxs[i]; ++j) {
            os << get_card_name(i);
        }
    }
    os << std::endl;
}

static void print_cardsunit(cardsunit* us, std::ostream& os) {
    for(int i = 0; i < us->count; ++i) {
        for(int j = 0; j < us->units[i].count; ++j) {
            os << get_card_name(us->units[i].ids[j]);
        }
        os << ",";
    }
    os << std::endl;
}
