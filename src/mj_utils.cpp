#include "mj_utils.h"

int get_card_shape_byidx(char idx) {
    if(idx == JOKER_INDEX) return SHAPE_JOKER;
    return idx / 9;
}

int get_card_value_byidx(char idx) {
    if(idx == JOKER_INDEX) return 10;// 财神不应该有牌面值
    if(idx >= HAND_CARDIDX_LAY_FENGSTART) return idx-HAND_CARDIDX_LAY_FENGSTART+1;
    return idx%9 + 1;
}

const char *get_shape_name(char shape) {
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

const char *get_card_name(char idx) {
    static const char* gTable[] = {
        "Wan(1)","Wan(2)","Wan(3)","Wan(4)","Wan(5)","Wan(6)","Wan(7)","Wan(8)","Wan(9)",// 0~8
        "Tiao(1)","Tiao(2)","Tiao(3)","Tiao(4)","Tiao(5)","Tiao(6)","Tiao(7)","Tiao(8)","Tiao(9)",// 9~17
        "Dong(1)","Dong(2)","Dong(3)","Dong(4)","Dong(5)","Dong(6)","Dong(7)","Dong(8)","Dong(9)",// 18~26
        "Feng(1)","Feng(2)","Feng(3)","Feng(4)","Feng(5)","Feng(6)","Feng(7)",          // 27~33
        "JOKER_INDEX"
    };
    static const char* gTable_1[] = {
        "Wan(-1)","Wan(-2)","Wan(-3)","Wan(-4)","Wan(-5)","Wan(-6)","Wan(-7)","Wan(-8)","Wan(-9)",// 0~8
        "Tiao(-1)","Tiao(-2)","Tiao(-3)","Tiao(-4)","Tiao(-5)","Tiao(-6)","Tiao(-7)","Tiao(-8)","Tiao(-9)",// 9~17
        "Dong(-1)","Dong(-2)","Dong(-3)","Dong(-4)","Dong(-5)","Dong(-6)","Dong(-7)","Dong(-8)","Dong(-9)",// 18~26
        "Feng(-1)","Feng(-2)","Feng(-3)","Feng(-4)","Feng(-5)","Feng(-6)","Feng(-7)",          // 27~33
        "JOKER_INDEX"
    };
    if(idx >= 0) {
        return gTable[idx];
    }
    return gTable_1[-idx];
}
