#include "mj_algo1.h"
#include "test.h"
#include <cstring>
#include <array>

static bool _canhu_4m1j_backtrace_nj(cardidxs* idx, bool& j) {
    if(idx->count == 0) {
        return j;
    }
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        // 刻字
        if(idx->idxs[i] >= 3) {
            idx->idxs[i] -= 3;
            idx->count -= 3;
            bool ok = _canhu_4m1j_backtrace_nj(idx, j);
            idx->count += 3;
            idx->idxs[i] += 3;   
            if(ok) {
                return ok;
            }
        }
        // 将
        if(idx->idxs[i] >= 2 && !j) {
            idx->idxs[i] -= 2;
            idx->count -= 2;
            j = true;
            bool ok = _canhu_4m1j_backtrace_nj(idx, j);
            j = false;
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
            bool ok = _canhu_4m1j_backtrace_nj(idx, j);
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[i+1] += 1;
            idx->idxs[i+2] += 1;
            if(ok) {
                return ok;
            }
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
    bool jiang = false;
    return _canhu_7j_backtrace_nj(c) || _canhu_4m1j_backtrace_nj(c,jiang);
}


static bool _travel_4m1j_backtrace_nj(cardidxs* idx, bool& j,cardsunit*u, bool (*f)(cardsunit*)) {
    if(idx->count == 0) {
        return f(u);
    }
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(idx->idxs[i] == 0) {
            continue;
        }
        // 刻字
        if(idx->idxs[i] >= 3) {
            idx->idxs[i] -= 3;
            idx->count -= 3;
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_KEZI_T, i, i, i);
            bool ok = _travel_4m1j_backtrace_nj(idx, j, u, f);
            pop_cardsunit_item(u);
            idx->count += 3;
            idx->idxs[i] += 3;   
            if(ok) {
                return ok;
            }
        }
        // 将
        if(idx->idxs[i] >= 2 && !j) {
            idx->idxs[i] -= 2;
            idx->count -= 2;
            j = true;
            init_unititem(grab_cardsunit_item(u), UNIT_ITEM_JIANG_T, i, i);
            bool ok = _travel_4m1j_backtrace_nj(idx, j, u, f);
            pop_cardsunit_item(u);
            j = false;
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
            bool ok = _travel_4m1j_backtrace_nj(idx, j, u, f);
            pop_cardsunit_item(u);
            idx->count += 3;
            idx->idxs[i] += 1;
            idx->idxs[i+1] += 1;
            idx->idxs[i+2] += 1;
            if(ok) {
                return ok;
            }
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
    _travel_4m1j_backtrace_nj(c,jiang,&units,f);
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

void test_algo1()
{
    TEST_CALL(test_canhu_nojoker);
    TEST_CALL(test_travel_all_hu_nojoker);
}
