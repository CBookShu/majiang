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
    108~114 东南西北中发白
    115~121 东南西北中发白
    122~128 东南西北中发白
    128~135 东南西北中发白

    36 cardidx:
    0~8     万,
    9~17    条,
    18~26   筒,
    27~33   风板,
    34      财神,
    toal: 35
*/

#define TOAL_CARDS                  135
#define TOAL_CARDS_NOJOKER          132
#define HAND_CARDS_COUNT            14
#define HAND_CARDIDX_LAY            35
#define HAND_CARDIDX_LAY_NOJOKER    34
#define HAND_CARDIDX_LAY_FENGSTART  27
#define UNIT_CARDS_MAX_COUNT        4
#define UNIT_MAX_COUNT              7
#define JOKER_MAX                   4

#define W(n)    (n-1)
#define T(n)    (9+n-1)
#define D(n)    (18+n-1)
#define F(n)    (27+n-1)


#define SHAPE_WAN                   0
#define SHAPE_TIAO                  1
#define SHAPE_DONG                  2
#define SHAPE_FENG                  3
#define SHAPE_JOKER                 4
#define ONE_COLOR_COUNT             36
#define JOKER_INDEX                 34         

// 获取牌id的花色
int get_card_shape_byidx(char idx);

// 获取牌面值
int get_card_value_byidx(char idx);

// 获取牌的名字
const char* get_shape_name(char shape);

const char* get_card_name(char idx);


typedef struct cardidxs {
    char idxs[HAND_CARDIDX_LAY];
    char count;
}cardidxs;

typedef struct cardsunititem {
    char ids[UNIT_CARDS_MAX_COUNT];
    char count;
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
    char type;
}cardsunititem;

typedef struct cardsunit {
    struct cardsunititem units[UNIT_MAX_COUNT];
    char count;
}cardsunit;

static void idxs_add(cardidxs* c, char idx, char add) {
    (c)->idxs[idx]+=add;(c)->count+=add;
}

template <typename...Args>
static void init_unititem(cardsunititem* u, char t, Args&&... is) {
    u->type = t;
    char i = 0;
    auto f = [&](auto v){
        u->ids[i++] = v;
    };
    (..., f(is));
    u->count = i;
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
