#pragma once
#include "utils.h"

/*
    麻将回溯计算：
    1.  判断通用的4M+J和7J 胡
*/


bool canhu_nojoker(cardidxs* c);

void travel_all_hu_nojoker(cardidxs* c, bool (*f)(cardsunit*));

bool canhu(cardidxs* c);

void travel_all_hu(cardidxs* c, bool (*f)(cardsunit*));



void test_algo1();