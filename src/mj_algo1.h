#pragma once
#include "utils.h"

/*
    麻将回溯计算：
    1.  判断通用的4M+J和7J 胡
    2.  手牌听牌的计算
    3.  当前所有胡可能性的遍历
*/


bool canhu_nojoker(cardidxs* c);

void travel_all_hu_nojoker(cardidxs* c, bool (*f)(cardsunit*));


void test_algo1();