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
        "W(1)","W(2)","W(3)","W(4)","W(5)","W(6)","W(7)","W(8)","W(9)",// 0~8
        "T(1)","T(2)","T(3)","T(4)","T(5)","T(6)","T(7)","T(8)","T(9)",// 9~17
        "D(1)","D(2)","D(3)","D(4)","D(5)","D(6)","D(7)","D(8)","D(9)",// 18~26
        "F(1)","F(2)","F(3)","F(4)","F(5)","F(6)","F(7)",          // 27~33
        "JOKER_INDEX"
    };
    static const char* gTable_1[] = {
        "W(-1)","W(-2)","W(-3)","W(-4)","W(-5)","W(-6)","W(-7)","W(-8)","W(-9)",// 0~8
        "T(-1)","T(-2)","T(-3)","T(-4)","T(-5)","T(-6)","T(-7)","T(-8)","T(-9)",// 9~17
        "D(-1)","D(-2)","D(-3)","D(-4)","D(-5)","D(-6)","D(-7)","D(-8)","D(-9)",// 18~26
        "F(-1)","F(-2)","F(-3)","F(-4)","F(-5)","F(-6)","F(-7)",          // 27~33
        "JOKER_INDEX"
    };
    if(idx >= 0) {
        return gTable[idx];
    }
    return gTable_1[-idx];
}
