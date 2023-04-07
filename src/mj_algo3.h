#pragma once
#include "mj_utils.h"
#include "mj_algo3_detail.h"


bool canhu_3(cardidxs* c);
void travel_all_hu_3(cardidxs* c, bool (*f)(hu_card_units *));

void mj_algo3_test();