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
        "1W","2W","3W","4W","5W","6W","7W","8W","9W",// 0~8
        "1T","2T","3T","4T","5T","6T","7T","8T","9T",// 9~17
        "1D","2D","3D","4D","5D","6D","7D","8D","9D",// 18~26
        "1F","2F","3F","4F","5F","6F","7F",          // 27~33
        "JOKER"
    };
    static const char* gTable_1[] = {
        "-1W","-2W","-3W","-4W","-5W","-6W","-7W","-8W","-9W",// 0~8
        "-1T","-2T","-3T","-4T","-5T","-6T","-7T","-8T","-9T",// 9~17
        "-1D","-D","-3D","-4D","-5D","-6D","-7D","-8D","-9D",// 18~26
        "-1F","-2F","-3F","-4F","-5F","-6F","-7F",           // 27~33
        "-JOKER"
    };
    if(idx >= 0) {
        return gTable[idx];
    }
    return gTable_1[-idx];
}
