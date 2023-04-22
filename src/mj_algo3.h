#pragma once
#include "mj_utils.h"
#include "mj_algo3_detail.h"


bool canhu_3(cardidxs* c);
// TODO: 7对退化为mj_algo1【它的计算已经足够简单了】这样可以减少4M+J的代码量
void travel_all_hu_3(cardidxs* c, bool (*f)(hu_card_units *));