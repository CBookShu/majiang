#include "utils.h"

int get_card_shape(int id) {
    return id / ONE_COLOR_COUNT;
}

int get_card_shape_byidx(int idx) {
    return idx / 10;
}

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

int get_card_idx(int id) {
    if(id >= 108) {
        return JOKER_INDEX;
    } else {
        return get_card_shape(id) * 10 + get_card_value(id);
    }
}

const char *get_shape_name(int shape) {
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

const char *get_card_name(int idx) {
    static const char* gTable[] = {
        "","1W","2W","3W","4W","5W","6W","7W","8W","9W",// 0~9
        "","1T","2T","3T","4T","5T","6T","7T","8T","9T",// 10~19
        "","1D","2D","3D","4D","5D","6D","7D","8D","9D",// 20~29
        "","","","","","JOKER",                         // 30~35
    };
    static const char* gTable_1[] = {
        "","-1W","-2W","-3W","-4W","-5W","-6W","-7W","-8W","-9W",// 0~9
        "","-1T","-2T","-3T","-4T","-5T","-6T","-7T","-8T","-9T",// 10~19
        "","-1D","-2D","-3D","-4D","-5D","-6D","-7D","-8D","-9D",// 20~29
        "","","","","","JOKER",                         // 30~35
    };
    if(idx >= 0) {
        return gTable[idx];
    }
    return gTable_1[-idx];
}
