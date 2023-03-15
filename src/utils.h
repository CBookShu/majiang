#pragma once

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
#define HAND_CARDIDX_LAY_NOJOKER    29
#define UNIT_CARDS_MAX_COUNT        4
#define UNIT_MAX_COUNT              7

#define INVALID_ID                  -1
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
