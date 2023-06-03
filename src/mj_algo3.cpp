#include "mj_algo3.h"
#include "mj_algo3_detail.h"
#include "mj_utils.h"
#include "test.h"
#include "doctest.h"

typedef struct pack_calc_ctx {
    cardidxs* c;
    hand_card_units p;
    int jokerleft;
    bool (* complete_func)(pack_calc_ctx*);
    bool (* travel_func)(hu_card_units* u);
}pack_calc_ctx;

static bool _complete_canhu_func(pack_calc_ctx* ctx) {
    int joker = ctx->c->idxs[JOKER_INDEX];
    int sz_before = ctx->p.M.count + ctx->p.D.count + ctx->p.J.count + ctx->p.P.count;
    if(sz_before == 0) {
        // 只有财神
        for(int i = 0; i < (joker-2)/3; ++i) {
            auto* item = ctx->p.M.grap();
            *item = hui_M_3JK();
        }
        auto* item = ctx->p.J.grap();
        *item = hui_J_2JK();
        ctx->c->idxs[JOKER_INDEX] = 0;
    }

    int sz_after = ctx->p.M.count + ctx->p.D.count + ctx->p.J.count + ctx->p.P.count;
    bool res = false;
    if(sz_after) {
        do {
            int cur = calc_mdjp(&ctx->p);
            int* des;        
            auto n = get_hu_mdjp_des_4m1j(ctx->c->idxs[JOKER_INDEX], des);
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
            n = get_hu_mdjp_des_7j(ctx->c->idxs[JOKER_INDEX], des);
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
        ctx->c->idxs[JOKER_INDEX] = joker;
        for(int i = 0; i < (joker-2)/3; ++i) {
            ctx->p.M.pop();
        }
        ctx->p.J.pop();
    }
    return res;
}

static bool _complete_travel_func(pack_calc_ctx* ctx) {
    int joker = ctx->c->idxs[JOKER_INDEX];
    int sz_before = ctx->p.M.count + ctx->p.D.count + ctx->p.J.count + ctx->p.P.count;
    if(sz_before == 0) {
        // 只有财神
        for(int i = 0; i < (joker-2)/3; ++i) {
            auto* item = ctx->p.M.grap();
            *item = hui_M_3JK();
        }
        auto* item = ctx->p.J.grap();
        *item = hui_J_2JK();
        ctx->c->idxs[JOKER_INDEX] = 0;
    }

    int sz_after = ctx->p.M.count + ctx->p.D.count + ctx->p.J.count + ctx->p.P.count;
    bool res = false;
    if(sz_after) {
        mix_hu_mdjp_travel(ctx->c->idxs[JOKER_INDEX], &ctx->p, ctx->travel_func);
    }
    if(sz_before == 0) {
        // 还原
        ctx->c->idxs[JOKER_INDEX] = joker;
        for(int i = 0; i < (joker-2)/3; ++i) {
            ctx->p.M.pop();
        }
        ctx->p.J.pop();
    }
    return res;
}


static bool _pack_func(pack_calc_ctx* ctx) {
    int pos = -1;
    for(int i = 0; i < HAND_CARDIDX_LAY_NOJOKER; ++i) {
        if(ctx->c->idxs[i] > 0) {
            pos = i;
            break;
        }
    }
    if(pos == -1) {
        return ctx->complete_func(ctx);
    }

    int v = get_card_value_byidx(pos);
    // M刻字
    if (ctx->c->idxs[pos] >= 3) {
        idxs_add(ctx->c, pos, -3);
        auto* item = ctx->p.M.grap();
        hui_init(item, {pos,pos,pos}, UIT_M, M_KEZI);
        auto r = _pack_func(ctx);
        idxs_add(ctx->c, pos, 3);
        ctx->p.M.pop();
        if (r) {
            return true;
        }
    }
    // M顺
    if(pos < HAND_CARDIDX_LAY_FENGSTART) {
        if(v <= 7 && ctx->c->idxs[pos+1] > 0 && ctx->c->idxs[pos+2] > 0) {
            UnitSubType subtype = M_ZHONG;
            if(v == 1 || v == 7) {
                subtype = M_BIAN;
            }
            idxs_add(ctx->c, pos, -1);
            idxs_add(ctx->c, pos+1, -1);
            idxs_add(ctx->c, pos+2, -1);
            auto* item = ctx->p.M.grap();
            hui_init(item, {pos, pos+1, pos+2}, UIT_M, subtype);
            auto r = _pack_func(ctx);
            idxs_add(ctx->c, pos, 1);
            idxs_add(ctx->c, pos+1, 1);
            idxs_add(ctx->c, pos+2, 1);
            ctx->p.M.pop();
            if(r) {
                return true;
            }
        }

        // D 连和坎
        // 没有Joker的时候，就无法组成D
        if(ctx->jokerleft > 0) {
            // 连
            if(v <= 8 && ctx->c->idxs[pos+1] > 0) {
                UnitSubType subtype = D_LIAN_ZHONG;
                if(v == 1 || v == 8) {
                    subtype = D_LIAN_BIAN;
                }
                ctx->jokerleft--;
                idxs_add(ctx->c, pos, -1);
                idxs_add(ctx->c, pos+1, -1);
                auto* item = ctx->p.D.grap();
                hui_init(item, {pos, pos+1}, UIT_D, subtype);
                auto r = _pack_func(ctx);
                ctx->jokerleft++;
                idxs_add(ctx->c, pos, 1);
                idxs_add(ctx->c, pos+1, 1);
                ctx->p.D.pop();
                if(r) {
                    return true;
                }
            }
            // 坎
            if(v <= 7 && ctx->c->idxs[pos+2] > 0) {
                UnitSubType subtype = D_KAN_ZHONG;
                if(v == 1 || v == 7) {
                    subtype = D_KAN_BIAN;
                }
                ctx->jokerleft--;
                idxs_add(ctx->c, pos, -1);
                idxs_add(ctx->c, pos+2, -1);
                auto* item = ctx->p.D.grap();
                hui_init(item, {pos, pos+2}, UIT_D, subtype);
                auto r = _pack_func(ctx);
                ctx->jokerleft++;
                idxs_add(ctx->c, pos, 1);
                idxs_add(ctx->c, pos+2, 1);
                ctx->p.D.pop();
                if(r) {
                    return true;
                }
            }
        }
    }

    // 将
    if (ctx->c->idxs[pos] >= 2) {
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
        idxs_add(ctx->c, pos, -1);
        idxs_add(ctx->c, pos, -1);
        auto* item = ctx->p.J.grap();
        hui_init(item, {pos, pos}, UIT_J, subtype);
        auto r = _pack_func(ctx);
        idxs_add(ctx->c, pos, 1);
        idxs_add(ctx->c, pos, 1);
        ctx->p.J.pop();
        if(r) {
            return true;
        }
    }

    // 每个单张都需要Joker最终才能胡！
    if(ctx->c->idxs[pos] == 1 && ctx->jokerleft > 0) {
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
        ctx->jokerleft--;
        idxs_add(ctx->c, pos, -1);
        auto* item = ctx->p.P.grap();
        hui_init(item, {pos}, UIT_P, subtype);
        auto r = _pack_func(ctx);
        ctx->jokerleft++;
        idxs_add(ctx->c, pos, 1);
        ctx->p.P.pop();
        if(r) {
            return true;
        }
    }

    return false;
}

bool canhu_3(cardidxs *c)
{
    pack_calc_ctx ctx;
    zero_struct(ctx);
    ctx.c = c;
    ctx.jokerleft = c->idxs[JOKER_INDEX];
    ctx.complete_func = _complete_canhu_func;
    return _pack_func(&ctx);
}

void travel_all_hu_3(cardidxs *c, bool (*f)(hu_card_units *))
{
    pack_calc_ctx ctx;
    zero_struct(ctx);
    ctx.c = c;
    ctx.jokerleft = c->idxs[JOKER_INDEX];
    ctx.complete_func = _complete_travel_func;
    ctx.travel_func = f;
    _pack_func(&ctx);
}

TEST_SUITE_BEGIN("mj_alog3");

TEST_CASE("test_hu_units") {
    hu_card_units hcu;
    zero_struct(hcu);
    auto* item = hcu.M.grap();
    hui_init(item, {1,2,3}, UIT_M, M_ZHONG);
    item = hcu.M.grap();
    hui_init(item, {4,4,4}, UIT_M, M_KEZI);
    print_hui_array(&hcu.M, std::cout);
}

TEST_CASE("test_canhu") {
    struct cardidxs idxs;
    zero_struct(idxs);
    // 2W3W,4W4W4W,8W8W,1T2T3T,6T7T8T,JOKER
    idxs_add_args(&idxs, Wan(2),Wan(3),Wan(4),Wan(4),Wan(4),Wan(8),Wan(8),
    Tiao(1),Tiao(2),Tiao(3),Tiao(6),Tiao(7),Tiao(8),JOKER_INDEX);
    REQUIRE(canhu_3(&idxs));

    zero_struct(idxs);
    // 1W1W,2W2W,3W3W,4W4W,7T7T7T,9T9T,JOKER
    idxs_add(&idxs, Wan(1), 2);idxs_add(&idxs, Wan(2), 2);idxs_add(&idxs, Wan(3), 2);
    idxs_add(&idxs, Tiao(4), 2);idxs_add(&idxs, Tiao(7), 3);idxs_add(&idxs, Tiao(9), 2);
    idxs_add(&idxs, JOKER_INDEX, 1);
    REQUIRE(canhu_3(&idxs));

    zero_struct(idxs);
    // 1W1W1W,2W2W,3W3W3W,7W7W,7W8W9W,JOKER
    idxs_add(&idxs, Wan(1), 3);idxs_add(&idxs, Wan(2), 2);idxs_add(&idxs, Wan(3), 3);
    idxs_add(&idxs, Wan(7), 3);idxs_add(&idxs, Wan(8), 1);idxs_add(&idxs, Wan(9), 1);
    idxs_add(&idxs, JOKER_INDEX, 1);
    REQUIRE(canhu_3(&idxs));
}

TEST_CASE("test_travelhu") {
    // TODO: 未完成4,5,6 joker的代码
    // 此外 travel_all_hu_3 回调函数的参数和mj_algo1的有不同，可以兼容一下
}

TEST_SUITE_END;