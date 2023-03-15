#pragma once
#include "utils.h"

/*
    麻将回溯计算：
    1.  判断通用的4M+J和7J 胡
    2.  手牌听牌的计算
    3.  当前所有胡可能性的遍历
*/

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
}cardsunititem;

typedef struct cardsunit {
    struct cardsunititem units[UNIT_MAX_COUNT];
    int count;
}cardsunit;

void ids2idxs(cardids* c1, cardidxs* c2);
#define idxs_add(c, idx, add)       (c)->idxs[idx]+=add;(c)->count+=add;
#define init_unititem(u)            (u)->count=0;memset((u)->ids,INVALID_ID,std::size((u)->ids))
#define init_cardsunit(us)          (us)->count=0;do {for(auto&u:(us)->units){init_unititem(&u);}} while(0)

bool canhu_nojoker(cardidxs* c);

void travel_all_hu(cardidxs* c, bool (*f)(cardsunit*));