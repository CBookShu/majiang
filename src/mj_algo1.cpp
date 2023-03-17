#include "mj_algo1.h"
#include "test.h"
#include <cstring>
#include <array>
#include <algorithm>

static bool _canhu_4m1j_backtrace_nj(cardidxs* idx, int b, bool&& jiang) {
    if(idx->count == 0) {
        return jiang;
    }
    int i = 0;
    for(i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] != 0) {
            break;
        }
    }
    // 刻字
    if(idx->idxs[i] >= 3) {
        idx->idxs[i] -= 3;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace_nj(idx,i,std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 3;   
        if(ok) {
            return ok;
        }
    }
    // 将
    if(idx->idxs[i] >= 2 && !jiang) {
        idx->idxs[i] -= 2;
        idx->count -= 2;
        jiang = true;
        bool ok = _canhu_4m1j_backtrace_nj(idx,i, std::forward<bool>(jiang));
        jiang = false;
        idx->count += 2;
        idx->idxs[i] += 2;
        if(ok) {
            return ok;
        }
    }
    // 顺子
    int value = i % 10;
    if(value <= 7 && idx->idxs[i + 1] > 0 && idx->idxs[i + 2] > 0) {
        idx->idxs[i] -= 1;
        idx->idxs[i+1] -= 1;
        idx->idxs[i+2] -= 1;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace_nj(idx,i, std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[i+1] += 1;
        idx->idxs[i+2] += 1;
        if(ok) {
            return ok;
        }
    }   
    return false;
}

static bool _canhu_7j_backtrace_nj(cardidxs* idx) {
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        if(idx->idxs[i] & 0x1) {
            // 奇数张，就无法组成7对
            return false;
        }
    }
    return true;
}

bool canhu_nojoker(cardidxs *c)
{
    // 4 M + 1 J = HU
    // 7 J = HU
    return _canhu_7j_backtrace_nj(c) || _canhu_4m1j_backtrace_nj(c,0,false);
}

static bool _travel_4m1j_backtrace_nj(cardidxs* idx, bool&& jiang, int b, cardsunit*u, bool (*f)(cardsunit*)) {
    if(idx->count == 0) {
        return f(u);
    }
    int i = 0;
    for(i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] != 0) {
            break;
        }
    }
    // 刻字
    if(idx->idxs[i] >= 3) {
        idx->idxs[i] -= 3;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, i, i, i);
        bool ok = _travel_4m1j_backtrace_nj(idx, std::forward<bool>(jiang), i, u, f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 3;   
        if(ok) {
            return ok;
        }
    }
    // 将
    if(idx->idxs[i] >= 2 && !jiang) {
        idx->idxs[i] -= 2;
        idx->count -= 2;
        jiang = true;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
        bool ok = _travel_4m1j_backtrace_nj(idx, std::forward<bool>(jiang), i, u, f);
        pop_cardsunit_item(u);
        jiang = false;
        idx->count += 2;
        idx->idxs[i] += 2;
        if(ok) {
            return ok;
        }
    }
    // 顺子
    int value = i % 10;
    if(value <= 7 && idx->idxs[i + 1] > 0 && idx->idxs[i + 2] > 0) {
        idx->idxs[i] -= 1;
        idx->idxs[i+1] -= 1;
        idx->idxs[i+2] -= 1;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i+1,i+2);
        bool ok = _travel_4m1j_backtrace_nj(idx, std::forward<bool>(jiang), i, u, f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[i+1] += 1;
        idx->idxs[i+2] += 1;
        if(ok) {
            return ok;
        }
    }
    return false;
}

static bool _travel_7j_backtrace_nj(cardidxs* idx,cardsunit*u, bool (*f)(cardsunit*)) {
    
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        if(idx->idxs[i] & 0x1) {
            // 奇数张，就无法组成7对
            return false;
        }

        if (idx->idxs[i] == 2) {
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
        } else if (idx->idxs[i] == 4) {
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
        }
    }
    if(u->count == 7) {
        f(u);
    }
    return true;
}

void travel_all_hu_nojoker(cardidxs* c, bool (*f)(cardsunit*))
{
    // 4 M + 1 J = HU
    // 7 J = HU
    bool jiang = false;
    cardsunit units;
    init_cardsunit(&units);
    _travel_7j_backtrace_nj(c,&units,f);
    init_cardsunit(&units);
    _travel_4m1j_backtrace_nj(c,false,0,&units,f);
    return;
}


static bool _canhu_7j_backtrace(cardidxs* idx) {
    int joker = idx->idxs[JOKER_INDEX];
    for (int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        
        if((idx->idxs[i] & 0x1)) {
            // 奇数，需要使用一次joker
            if(joker == 0) {
                return false;
            }
            joker--;
        }
    }
    assert((joker&0x1)==0);
    return true;
}

static bool _canhu_4m1j_backtrace(cardidxs* idx, int b, bool&& jiang) {
    if(idx->count == 0) {
        return jiang;
    }
    
    int i = 0;
    for (i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] > 0) {
            break;
        }
    }
    if(i == HAND_CARDIDX_LAY_NOJOKER) {
        // just left joker
        return true;
    }

    int joker = idx->idxs[JOKER_INDEX];
    // 刻字
    if(idx->idxs[i] >= 3) {
        idx->idxs[i] -= 3;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace(idx,i,std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 3;   
        if(ok) {
            return ok;
        }
    }
    if(idx->idxs[i] >= 2 && joker >= 1) {
        idx->idxs[JOKER_INDEX] -= 1;
        idx->idxs[i] -= 2;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace(idx,i,std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 2;
        idx->idxs[JOKER_INDEX] += 1;
        if(ok) {
            return ok;
        }
    }
    if(idx->idxs[i] >=1 && joker >= 2) {
        idx->idxs[JOKER_INDEX] -= 2;
        idx->idxs[i] -= 1;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace(idx,i,std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[JOKER_INDEX] += 2;
        if(ok) {
            return ok;
        }
    }
    // 将
    if(idx->idxs[i] >= 2 && !jiang) {
        idx->idxs[i] -= 2;
        idx->count -= 2;
        jiang = true;
        bool ok = _canhu_4m1j_backtrace(idx,i, std::forward<bool>(jiang));
        jiang = false;
        idx->count += 2;
        idx->idxs[i] += 2;
        if(ok) {
            return ok;
        }
    }
    if(!jiang && idx->idxs[i] >=1 && joker >= 1) {
        idx->idxs[JOKER_INDEX] -= 1;
        idx->idxs[i] -= 1;
        idx->count -= 2;
        jiang = true;
        bool ok = _canhu_4m1j_backtrace(idx,i,std::forward<bool>(jiang));
        jiang = false;
        idx->count += 2;
        idx->idxs[i] += 1;
        idx->idxs[JOKER_INDEX] += 1;
        if(ok) {
            return ok;
        }
    }

    // 顺子
    int value = i % 10;
    if(value <= 7 && idx->idxs[i + 1] > 0 && idx->idxs[i + 2] > 0) {
        idx->idxs[i] -= 1;
        idx->idxs[i+1] -= 1;
        idx->idxs[i+2] -= 1;
        idx->count -= 3;
        bool ok = _canhu_4m1j_backtrace(idx,i, std::forward<bool>(jiang));
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[i+1] += 1;
        idx->idxs[i+2] += 1;
        if(ok) {
            return ok;
        }
    }
    if(value <= 7 && joker > 0)
    {
        if(idx->idxs[i+1]==0 && idx->idxs[i+2]!=0) {
            idx->idxs[i] -= 1;
            idx->idxs[JOKER_INDEX] -= 1;
            idx->idxs[i+2] -= 1;
            idx->count -= 3;
            bool ok = _canhu_4m1j_backtrace(idx,i, std::forward<bool>(jiang));
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[JOKER_INDEX] += 1;
            idx->idxs[i+2] += 1;
            if(ok) {
                return ok;
            }
        }
        if(idx->idxs[i+1]!=0&&idx->idxs[i+2]==0) {
            idx->idxs[i] -= 1;
            idx->idxs[i+1] -= 1;
            idx->idxs[JOKER_INDEX] -= 1;
            idx->count -= 3;
            bool ok = _canhu_4m1j_backtrace(idx,i, std::forward<bool>(jiang));
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[i+1] += 1;
            idx->idxs[JOKER_INDEX] += 1;
            if(ok) {
                return ok;
            }
        }
        if(idx->idxs[i+1]==0&&idx->idxs[i+2]==0&&joker>=2) {
            idx->idxs[i] -= 1;
            idx->idxs[JOKER_INDEX] -= 2;
            idx->count -= 3;
            bool ok = _canhu_4m1j_backtrace(idx,i, std::forward<bool>(jiang));
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[JOKER_INDEX] += 2;
            if(ok) {
                return ok;
            }
        }
    }
    return false;
}

bool canhu(cardidxs *c)
{
    return _canhu_7j_backtrace(c) || _canhu_4m1j_backtrace(c,0,false);
}

static bool _travel_7j_backtrace(cardidxs* idx, cardsunit* u, bool (*f)(cardsunit *)) {
    int joker = idx->idxs[JOKER_INDEX];
    for (int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        
        int count = idx->idxs[i];
        for(int i = 0; i < count / 2; ++i) {
            cardsunititem* item = grab_cardsunit_item(u);
            init_unititem(item, UNIT_ITEM_JIANG_T, i, i);
        }
        if((idx->idxs[i] & 0x1)) {
            // 奇数，需要使用一次joker
            if(joker == 0) {
                return false;
            }
            joker--;
            cardsunititem* item = grab_cardsunit_item(u);
            init_unititem(item, UNIT_ITEM_JIANG_T, i, -i);
        }
    }
    assert((joker & 0x1) ==0);
    for (int i = 0; i < joker / 2; ++i) {
        cardsunititem* item = grab_cardsunit_item(u);
        init_unititem(item, UNIT_ITEM_JIANG_T, JOKER_INDEX, JOKER_INDEX);
    }
    f(u);
    return true;
}

static bool _travel_4m1j_backtrace(cardidxs* idx,bool&& jiang,int b,cardsunit* u, bool (*f)(cardsunit *)) {
    if(idx->count == 0) {
        return f(u);
    }
    
    int i = 0;
    for (i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] > 0) {
            break;
        }
    }
    if(i == HAND_CARDIDX_LAY_NOJOKER) {
        // just left joker
        int count = idx->idxs[JOKER_INDEX];
        for(int i = 0; i < count / 3; ++i) {
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, JOKER_INDEX, JOKER_INDEX, JOKER_INDEX);
        }
        assert(count%3==2||count%3==0);
        if(count%3==2) {
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, JOKER_INDEX, JOKER_INDEX);
        }
        return f(u);
    }

    int joker = idx->idxs[JOKER_INDEX];
    // 刻字
    if(idx->idxs[i] >= 3) {
        idx->idxs[i] -= 3;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, i,i,i);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 3;   
        if(ok) {
            return ok;
        }
    }
    if(idx->idxs[i] >= 2 && joker >= 1) {
        idx->idxs[JOKER_INDEX] -= 1;
        idx->idxs[i] -= 2;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, i,i,-i);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 2;
        idx->idxs[JOKER_INDEX] += 1;
        if(ok) {
            return ok;
        }
    }
    if(idx->idxs[i] >=1 && joker >= 2) {
        idx->idxs[JOKER_INDEX] -= 2;
        idx->idxs[i] -= 1;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, i,-i,-i);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[JOKER_INDEX] += 2;
        if(ok) {
            return ok;
        }
    }
    // 将
    if(idx->idxs[i] >= 2 && !jiang) {
        idx->idxs[i] -= 2;
        idx->count -= 2;
        jiang = true;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        jiang = false;
        idx->count += 2;
        idx->idxs[i] += 2;
        if(ok) {
            return ok;
        }
    }
    if(!jiang && idx->idxs[i] >=1 && joker >= 1) {
        idx->idxs[JOKER_INDEX] -= 1;
        idx->idxs[i] -= 1;
        idx->count -= 2;
        jiang=true;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i,-i);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        jiang=false;
        idx->count += 2;
        idx->idxs[i] += 1;
        idx->idxs[JOKER_INDEX] += 1;
        if(ok) {
            return ok;
        }
    }

    // 顺子
    int value = i % 10;
    if(value <= 7 && idx->idxs[i + 1] > 0 && idx->idxs[i + 2] > 0) {
        idx->idxs[i] -= 1;
        idx->idxs[i+1] -= 1;
        idx->idxs[i+2] -= 1;
        idx->count -= 3;
        init_unititem(grab_cardsunit_item(u), UNIT_ITEM_SHUN_T, i,i+1,i+2);
        bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
        pop_cardsunit_item(u);
        idx->count += 3;
        idx->idxs[i] += 1;
        idx->idxs[i+1] += 1;
        idx->idxs[i+2] += 1;
        if(ok) {
            return ok;
        }
    }
    if(value <= 7 && joker > 0)
    {
        if(idx->idxs[i+1]==0 && idx->idxs[i+2]!=0) {
            idx->idxs[i] -= 1;
            idx->idxs[JOKER_INDEX] -= 1;
            idx->idxs[i+2] -= 1;
            idx->count -= 3;
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_SHUN_T, i,-(i+1),i+2);
            bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
            pop_cardsunit_item(u);
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[JOKER_INDEX] += 1;
            idx->idxs[i+2] += 1;
            if(ok) {
                return ok;
            }
        }
        if(idx->idxs[i+1]!=0&&idx->idxs[i+2]==0) {
            idx->idxs[i] -= 1;
            idx->idxs[i+1] -= 1;
            idx->idxs[JOKER_INDEX] -= 1;
            idx->count -= 3;
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_SHUN_T, i,i+1,-(i+2));
            bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
            pop_cardsunit_item(u);
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[i+1] += 1;
            idx->idxs[JOKER_INDEX] += 1;
            if(ok) {
                return ok;
            }
        }
        if(idx->idxs[i+1]==0&&idx->idxs[i+2]==0&&joker>=2) {
            idx->idxs[i] -= 1;
            idx->idxs[JOKER_INDEX] -= 2;
            idx->count -= 3;
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_SHUN_T, i,-(i+1),-(i+2));
            bool ok = _travel_4m1j_backtrace(idx,std::forward<bool>(jiang),i,u,f);
            pop_cardsunit_item(u);
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[JOKER_INDEX] += 2;
            if(ok) {
                return ok;
            }
        }
    }
    return false;
}

void travel_all_hu(cardidxs *c, bool (*f)(cardsunit *))
{
    // 4 M + 1 J = HU
    // 7 J = HU
    bool jiang = false;
    cardsunit units;
    init_cardsunit(&units);
    _travel_7j_backtrace(c,&units,f);
    init_cardsunit(&units);
    _travel_4m1j_backtrace(c,false,0,&units,f);
    return;
}

void test_canhu_nojoker() {
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 1W2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T
    idxs_add(&idxs, 1, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, 4, 3);idxs_add(&idxs, 8, 2);
    idxs_add(&idxs, 11, 1);idxs_add(&idxs, 12, 1);idxs_add(&idxs, 13, 1);
    idxs_add(&idxs, 16, 1);idxs_add(&idxs, 17, 1);idxs_add(&idxs, 18, 1);
    assert(canhu_nojoker(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T7T,9T9T
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 2);
    idxs_add(&idxs, 4, 2);idxs_add(&idxs, 17, 4);idxs_add(&idxs, 19, 2);
    assert(canhu_nojoker(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W1W,2W2W2W,3W3W3W,7W7W,7W8W9W
    // 1W2W3W,1W2W3W,1W2W3W,7W7W,7W8W9W
    idxs_add(&idxs, 1, 3);idxs_add(&idxs, 2, 3);idxs_add(&idxs, 3, 3);
    idxs_add(&idxs, 7, 3);idxs_add(&idxs, 8, 1);idxs_add(&idxs, 9, 1);
    assert(canhu_nojoker(&idxs));
}

void test_travel_all_hu_nojoker() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 1W2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T
    idxs_add(&idxs, 1, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, 4, 3);idxs_add(&idxs, 8, 2);
    idxs_add(&idxs, 11, 1);idxs_add(&idxs, 12, 1);idxs_add(&idxs, 13, 1);
    idxs_add(&idxs, 16, 1);idxs_add(&idxs, 17, 1);idxs_add(&idxs, 18, 1);
    travel_all_hu_nojoker(&idxs, f);

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T7T,9T9T
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 2);
    idxs_add(&idxs, 4, 2);idxs_add(&idxs, 17, 4);idxs_add(&idxs, 19, 2);
    travel_all_hu_nojoker(&idxs, f);

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W1W,2W2W2W,3W3W3W,7W7W,7W8W9W
    // 1W2W3W,1W2W3W,1W2W3W,7W7W,7W8W9W
    idxs_add(&idxs, 1, 3);idxs_add(&idxs, 2, 3);idxs_add(&idxs, 3, 3);
    idxs_add(&idxs, 7, 3);idxs_add(&idxs, 8, 1);idxs_add(&idxs, 9, 1);
    travel_all_hu_nojoker(&idxs, f);
}

void test_rnd_travel_nojoker() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };
    std::default_random_engine e(std::random_device{}());
    int cards[TOAL_CARDS];
    for(int i = 0; i < TOAL_CARDS; ++i) {
        cards[i] = i;
    }

    int time = 0;
    while(true) {
        std::shuffle(std::begin(cards), std::end(cards), e);

        cardids ids;
        std::memset(&ids, 0, sizeof(ids));
        for(int i = 0; i < HAND_CARDS_COUNT; ++i) {
            ids_add(&ids, cards[i]);
        }
        cardidxs idx;
        ids2idxs(&ids, &idx);
        auto now = std::chrono::system_clock::now();
        if(canhu_nojoker(&idx)) {
            print_cardidx(&idx, std::cout);
            travel_all_hu_nojoker(&idx, f);
            auto after = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(after-now).count();
            if(diff > 2) {
                std::cout << "diff > 2ms" << std::endl;
                break;
            }
            if(++time > 5) {
                std::cout << "calc all < 2ms" << std::endl;
                break;
            }
        }
    }
}

void test_canhu() {
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T,JOKER
    idxs_add(&idxs, JOKER_INDEX, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, 4, 3);idxs_add(&idxs, 8, 2);
    idxs_add(&idxs, 11, 1);idxs_add(&idxs, 12, 1);idxs_add(&idxs, 13, 1);
    idxs_add(&idxs, 16, 1);idxs_add(&idxs, 17, 1);idxs_add(&idxs, 18, 1);
    assert(canhu(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T,9T9T,JOKER
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 2);
    idxs_add(&idxs, 4, 2);idxs_add(&idxs, 17, 3);idxs_add(&idxs, 19, 2);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu(&idxs));

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W1W,2W2W,3W3W3W,7W7W,7W8W9W,JOKER
    idxs_add(&idxs, 1, 3);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 3);
    idxs_add(&idxs, 7, 3);idxs_add(&idxs, 8, 1);idxs_add(&idxs, 9, 1);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu(&idxs));
}


void test_travel_all_hu() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T,JOKER
    idxs_add(&idxs, JOKER_INDEX, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, 4, 3);idxs_add(&idxs, 8, 2);
    idxs_add(&idxs, 11, 1);idxs_add(&idxs, 12, 1);idxs_add(&idxs, 13, 1);
    idxs_add(&idxs, 16, 1);idxs_add(&idxs, 17, 1);idxs_add(&idxs, 18, 1);
    travel_all_hu(&idxs, f);

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T,9T9T,JOKER
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 2);
    idxs_add(&idxs, 4, 2);idxs_add(&idxs, 17, 3);idxs_add(&idxs, 19, 2);
    idxs_add(&idxs, JOKER_INDEX, 1);
    travel_all_hu(&idxs, f);

    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W1W,2W2W,3W3W3W,7W7W,7W8W9W,JOKER
    idxs_add(&idxs, 1, 3);idxs_add(&idxs, 2, 2);idxs_add(&idxs, 3, 3);
    idxs_add(&idxs, 7, 3);idxs_add(&idxs, 8, 1);idxs_add(&idxs, 9, 1);
    idxs_add(&idxs, JOKER_INDEX, 1);
    travel_all_hu(&idxs, f);
}

void test_travel_1() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };
    struct cardidxs idxs;
    std::memset(&idxs, 0, sizeof(idxs));
    // 1W1W4W5W6W5T5T5T1D9DJOKERJOKERJOKERJOKER
    idxs_add(&idxs, JOKER_INDEX, 4);
    idxs_add(&idxs, 1, 2);idxs_add(&idxs, 4, 1);
    idxs_add(&idxs, 5, 1);idxs_add(&idxs, 6, 1);
    idxs_add(&idxs, 15, 3);idxs_add(&idxs, 21, 1);
    idxs_add(&idxs, 29, 1);
    travel_all_hu(&idxs, f);
}

void test_rnd_travel() {
    bool (*f)(cardsunit*) = nullptr;
    f = [](cardsunit* u){
        print_cardsunit(u, std::cout);
        return false;
    };
    std::default_random_engine e(std::random_device{}());
    int cards[TOAL_CARDS];
    for(int i = 0; i < TOAL_CARDS; ++i) {
        cards[i] = i;
    }

    int time = 0;
    while(true) {
        std::shuffle(std::begin(cards), std::end(cards), e);

        cardids ids; ids.count = 0;
        std::memset(&ids.ids, INVALID_ID, sizeof(ids.ids));
        for(int i = 0; i < HAND_CARDS_COUNT-3; ++i) {
            ids_add(&ids, cards[i]);
        }
        cardidxs idx;
        ids2idxs(&ids, &idx);
        idxs_add(&idx, JOKER_INDEX, 3);
        auto now = std::chrono::system_clock::now();
        if(canhu(&idx)) {
            if (idx.count != 14) {
                std::cout << "BAD" << std::endl;
            }
            print_cardidx(&idx, std::cout);
            travel_all_hu(&idx, f);
            auto after = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(after-now).count();
            if(diff > 2) {
                printf("cost %lldms\r\n", diff);
            }
            if(++time > 5) {
                break;
            }
        }
    }
}


void test_algo1()
{
    // TEST_CALL(test_canhu_nojoker);
    // TEST_CALL(test_travel_all_hu_nojoker);
    // TEST_CALL(test_rnd_travel_nojoker);
    

    // TEST_CALL(test_canhu);
    // TEST_CALL(test_travel_all_hu);
    TEST_CALL(test_travel_1);
    TEST_CALL(test_rnd_travel);
}
