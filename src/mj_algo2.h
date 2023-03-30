#pragma once
#include "mj_algo2_detail.h"


void gen_cache();

bool canhu_2(cardidxs* c);

void travel_all_hu_2(cardidxs* c, bool (*f)(cardsunit*));

void test_algo2();