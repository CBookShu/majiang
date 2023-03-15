#include "mj_algo1.h"
#include <string>

void ids2idxs(cardids *c1, cardidxs *c2) 
{
    memset(c2, 0, sizeof(*c2));
    for(int i = 0; i < std::size(c1->ids); ++i) {
        if(c1->ids[i] != INVALID_ID) {
            c2->idxs[get_card_idx(c1->ids[i])]++;
            c2->count++;
        }
    }
}

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
        if(idx->idxs[i] >= 2) {
            idx->idxs[i] -= 2;
            idx->count -= 2;
            j = true;
            bool ok = _canhu_4m1j_backtrace_nj(idx, j);
            j = false;
            idx->count += 2;
            idx->idxs[i] -= 2;
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
        if(idx->idxs[i] >= 2) {
            idx->idxs[i] -= 2;
            idx->count -= 2;
            j = true;
            bool ok = _canhu_4m1j_backtrace_nj(idx, j);
            j = false;
            idx->count += 2;
            idx->idxs[i] -= 2;
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

static bool _travel_7j_backtrace_nj(cardidxs* idx,cardsunit*u, bool (*f)(cardsunit*)) {
    
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


void travel_all_hu(cardidxs* c, bool (*f)(cardsunit*))
{
        // 4 M + 1 J = HU
    // 7 J = HU
    bool jiang = false;
    cardsunit units;
    init_cardsunit(&units);
    _travel_7j_backtrace_nj(c,&units,f);
    _travel_4m1j_backtrace_nj(c,jiang,&units,f);
    return;
}
