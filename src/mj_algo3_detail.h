#pragma once
#include "utils.h"
#include <cassert>

typedef struct hu_units_des {
    int M;
    int D;
    int J;
    int P;
}hu_units_des;

enum UnitType {
    UIT_NONE = 0,
    UIT_M,
    UIT_D,
    UIT_J,
    UIT_P
};

enum UnitSubType {
    M_BIAN = 0,         // 顺边张(123,789)
    M_ZHONG = 1,        // 顺中 (非边张的顺)
    M_KEZI = 2,         // 刻字
    M_2JK = 3,          // 一张牌+2财神
    M_3JK = 4,          // 3张财神

    D_LIAN_BIAN = 5,    // 连边张(12,89)
    D_LIAN_ZHONG = 6,   // 连中(连非边张)
    D_KAN_BIAN = 7,     // 坎边张(13,79)
    D_KAN_ZHONG = 8,    // 坎中(非坎边张)

    J_19 = 9,           // 对1，对9
    J_28 = 10,          // 对2，对8
    J_ZHONG = 11,       // 非1928对
    J_2JK = 12,         // 2张财神做将
    J_FB = 13,          // 风板将(它们单独拎出来，是因为无法成顺，可以评估它的价值)

    P_19 = 14,          // 单张19
    P_28 = 15,          // 单张28
    P_ZHONG = 16,       // 单张非1928
    P_FB = 17,          // 单张风板
};

typedef struct hu_unit_item {
    int idx[3]; //M,D,J,P 最多3张牌
    UnitType type;
    UnitSubType subtype;
}hu_unit_item;

typedef struct hu_units {
    hu_unit_item us[7]; // 4m+1j || 7j
    int count;
}hu_units;

int hui_card_count(UnitType type);

void hui_init(hu_unit_item* p, int card[], UnitType type, UnitSubType subtype);

hu_unit_item hui_P(int cardidx);

hu_unit_item hui_M_3JK();

hu_unit_item hui_J_2JK();

hu_unit_item hui_M_2JK(int cardidx);

// D+1JK=M
int hui_D2M(hu_unit_item* d, hu_unit_item ms[2]);

// J+1JK=M
hu_unit_item hui_J2M(hu_unit_item* j);

// P+1JK=J
hu_unit_item hui_P2J(hu_unit_item* p);

// P+2JK=M
hu_unit_item hui_P2M(hu_unit_item* p);

void hus_init(hu_units* p);

hu_unit_item* hus_grab_item(hu_units* p);

void hs_pop_item(hu_units* p);

int get_hu_mdjp_des(int joker, hu_units_des*& des);