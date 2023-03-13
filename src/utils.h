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

#define INVALID_ID                  -1
#define SHAPE_WAN                   0
#define SHAPE_TIAO                  1
#define SHAPE_DONG                  2
#define SHAPE_JOKER                 3
#define ONE_COLOR_COUNT             36
#define JOKER_INDEX                 35         

// 获取牌id的花色
int get_card_shape(int id) {
    return id / ONE_COLOR_COUNT;
}

int get_card_shape_byidx(int idx) {
    return idx / 10;
}

// 获取牌面值
int get_card_value(int id) {
    if(id < 108) {
        return id % 9 + 1;
    } else {
        return 10;
    }
}

int get_card_value_byidx(int idx) {
    return idx % 10;
}

// 获取牌idx
int get_card_idx(int id) {
    if(id >= 108) {
        return JOKER_INDEX;
    } else {
        return get_card_shape(id) * 10 + get_card_value(id);
    }
}

// 获取牌的名字
const char* get_shape_name(int shape) {
    switch (shape)
    {
    case SHAPE_WAN:
        return "W";
    case SHAPE_TIAO:
        return "T";
    case SHAPE_DONG:
        return "D";
    case SHAPE_JOKER:
        return "JK";
    default:
        break;
    }
    return "unkonw";
}

const char* get_card_name(int idx) {
    static const char* gTable[] = {
        "","1W","2W","3W","4W","5W","6W","7W","8W","9W",// 0~9
        "","1T","2T","3T","4T","5T","6T","7T","8T","9T",// 10~19
        "","1D","2D","3D","4D","5D","6D","7D","8D","9D",// 20~29
        "","","","","","JOKER",                         // 30~35
    };
    return gTable[idx];
}
