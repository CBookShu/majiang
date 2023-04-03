#include "mj_algo3.h"
#include "mj_algo3_detail.h"
#include "mj_utils.h"
#include "test.h"

static bool _complete_func(cardidxs*c,hand_card_units* p) {
    int joker = c->idxs[JOKER_INDEX];
    int sz_before = p->M.count + p->D.count + p->J.count + p->P.count;
    if(sz_before == 0) {
        // 只有财神
        for(int i = 0; i < (joker-2)/3; ++i) {
            auto* item = p->M.grap();
            *item = hui_M_3JK();
        }
        auto* item = p->J.grap();
        *item = hui_J_2JK();
        c->idxs[JOKER_INDEX] = 0;
    }

    int sz_after = p->M.count + p->D.count + p->J.count + p->P.count;
    bool res = false;
    if(sz_after) {
        do {
            int cur = calc_mdjp(p);
            int* des;        
            auto n = get_hu_mdjp_des_4m1j(c->idxs[JOKER_INDEX], des);
            for(int i = 0; i < n; ++i) {
                if(cur == des[i]) {
                    // 4m+1j 胡
                    res = true;
                    break;
                }
            }
            if(res) {
                break;
            }
            n = get_hu_mdjp_des_7j(c->idxs[JOKER_INDEX], des);
            for(int i = 0; i < n; ++i) {
            if(cur == des[i]) {
                // 7j 胡
                res = true;
                break;
            }
            }
        } while(0);
    }
    if(sz_before == 0) {
        // 还原
        c->idxs[JOKER_INDEX] = joker;
        for(int i = 0; i < (joker-2)/3; ++i) {
            p->M.pop();
        }
        p->J.pop();
    }
    return res;
}

static bool _pack_func(cardidxs*c, hand_card_units* p, int jokerleft) {
    int pos = -1;
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(c->idxs[i] > 0) {
            pos = i;
            break;
        }
    }
    if(pos == -1) {
        return _complete_func(c,p);
    }

    int v = get_card_value_byidx(pos);
    // M刻字
    if (c->idxs[pos] >= 3) {
        idxs_add(c, pos, -3);
        auto* item = p->M.grap();
        hui_init(item, {pos,pos,pos}, UIT_M, M_KEZI);
        auto r = _pack_func(c, p, jokerleft);
        idxs_add(c, pos, 3);
        p->M.pop();
        if (r) {
            return true;
        }
    }
    // M顺
    if(pos < HAND_CARDIDX_LAY_FENGSTART) {
        if(v <= 7 && c->idxs[pos+1] > 0 && c->idxs[pos+2] > 0) {
            UnitSubType subtype = M_ZHONG;
            if(v == 1 || v == 7) {
                subtype = M_BIAN;
            }
            idxs_add(c, pos, -1);
            idxs_add(c, pos+1, -1);
            idxs_add(c, pos+2, -1);
            auto* item = p->M.grap();
            hui_init(item, {pos, pos+1, pos+2}, UIT_M, subtype);
            auto r = _pack_func(c, p, jokerleft);
            idxs_add(c, pos, 1);
            idxs_add(c, pos+1, 1);
            idxs_add(c, pos+2, 1);
            p->M.pop();
            if(r) {
                return true;
            }
        }

        // D 连和坎
        // 没有Joker的时候，就无法组成D
        if(jokerleft > 0) {
            // 连
            if(v <= 8 && c->idxs[pos+1] > 0) {
                UnitSubType subtype = D_LIAN_ZHONG;
                if(v == 1 || v == 8) {
                    subtype = D_LIAN_BIAN;
                }
                jokerleft--;
                idxs_add(c, pos, -1);
                idxs_add(c, pos+1, -1);
                auto* item = p->D.grap();
                hui_init(item, {pos, pos+1}, UIT_D, subtype);
                auto r = _pack_func(c, p, jokerleft);
                jokerleft++;
                idxs_add(c, pos, 1);
                idxs_add(c, pos+1, 1);
                p->D.pop();
                if(r) {
                    return true;
                }
            }
            // 坎
            if(v <= 7 && c->idxs[pos+2] > 0) {
                UnitSubType subtype = D_KAN_ZHONG;
                if(v == 1 || v == 7) {
                    subtype = D_KAN_BIAN;
                }
                jokerleft--;
                idxs_add(c, pos, -1);
                idxs_add(c, pos+2, -1);
                auto* item = p->D.grap();
                hui_init(item, {pos, pos+2}, UIT_D, subtype);
                auto r = _pack_func(c, p, jokerleft);
                jokerleft++;
                idxs_add(c, pos, 1);
                idxs_add(c, pos+2, 1);
                p->D.pop();
                if(r) {
                    return true;
                }
            }
        }
    }

    // 将
    if (c->idxs[pos] >= 2) {
        UnitSubType subtype = J_ZHONG;
        if(pos >= HAND_CARDIDX_LAY_FENGSTART) {
            subtype = J_FB;
        } else {
            if(v == 1 || v == 9) {
                subtype = J_19;
            } else if(v == 2 || v == 8) {
                subtype = J_28;
            }
        }
        jokerleft--;
        idxs_add(c, pos, -1);
        idxs_add(c, pos, -1);
        auto* item = p->J.grap();
        hui_init(item, {pos, pos}, UIT_J, subtype);
        auto r = _pack_func(c, p, jokerleft);
        jokerleft++;
        idxs_add(c, pos, 1);
        idxs_add(c, pos, 1);
        p->J.pop();
        if(r) {
            return true;
        }
    }

    // 每个单张都需要Joker最终才能胡！
    if(c->idxs[pos] == 1 && jokerleft > 0) {
        UnitSubType subtype = P_ZHONG;
        if(pos >= HAND_CARDIDX_LAY_FENGSTART) {
            subtype = P_FB;
        } else {
            if(v == 1 || v == 9) {
                subtype = P_19;
            } else if(v == 2 || v == 8) {
                subtype = P_28;
            }
        }
        jokerleft--;
        idxs_add(c, pos, -1);
        auto* item = p->P.grap();
        hui_init(item, {pos}, UIT_P, subtype);
        auto r = _pack_func(c, p, jokerleft);
        jokerleft++;
        idxs_add(c, pos, 1);
        p->P.pop();
        if(r) {
            return true;
        }
    }

    return false;
}

bool canhu_3(cardidxs *c)
{
    int joker = c->idxs[JOKER_INDEX];
    hand_card_units hcu;
    zero_struct(hcu);
    return _pack_func(c, &hcu, joker);
}

static void test_hu_units() {
    hu_card_units hcu;
    zero_struct(hcu);
    auto* item = hcu.M.grap();
    hui_init(item, {1,2,3}, UIT_M, M_ZHONG);
    item = hcu.M.grap();
    hui_init(item, {4,4,4}, UIT_M, M_KEZI);
    print_hui_array(&hcu.M, std::cout);
}

static void test_canhu() {
    struct cardidxs idxs;
    zero_struct(idxs);
    // 2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T,JOKER
    idxs_add(&idxs, JOKER_INDEX, 1);idxs_add(&idxs, 2, 1);idxs_add(&idxs, 3, 1);
    idxs_add(&idxs, W(4), 3);idxs_add(&idxs, W(8), 2);
    idxs_add(&idxs, T(1), 1);idxs_add(&idxs, T(2), 1);idxs_add(&idxs, T(3), 1);
    idxs_add(&idxs, T(6), 1);idxs_add(&idxs, T(7), 1);idxs_add(&idxs, T(8), 1);
    assert(canhu_3(&idxs));

    zero_struct(idxs);
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T,9T9T,JOKER
    idxs_add(&idxs, W(1), 2);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 2);
    idxs_add(&idxs, T(4), 2);idxs_add(&idxs, T(7), 3);idxs_add(&idxs, T(9), 2);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu_3(&idxs));

    zero_struct(idxs);
    // 1W1W1W,2W2W,3W3W3W,7W7W,7W8W9W,JOKER
    idxs_add(&idxs, W(1), 3);idxs_add(&idxs, W(2), 2);idxs_add(&idxs, W(3), 3);
    idxs_add(&idxs, W(7), 3);idxs_add(&idxs, W(8), 1);idxs_add(&idxs, W(9), 1);
    idxs_add(&idxs, JOKER_INDEX, 1);
    assert(canhu_3(&idxs));
}

void mj_algo3_test()
{
    TEST_CALL(test_hu_units);
    TEST_CALL(test_canhu);
}
