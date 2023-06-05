#include "mj_algo3_detail.h"
#include "mj_utils.h"
#include <tuple>
#include <sstream>
#include "doctest.h"

int hui_count(UnitSubType type)
 {
    switch (type)
    {
    case M_BIAN:
    case M_KEZI:
    case M_2JK:
    case M_3JK:
    case M_ZHONG:
        return 3;
    case D_KAN_BIAN:
    case D_KAN_ZHONG:
    case D_LIAN_BIAN:
    case D_LIAN_ZHONG:
    case J_19:
    case J_28:
    case J_2JK:
    case J_FB:
    case J_ZHONG:
        return 2;
    case P_19:
    case P_28:
    case P_FB:
    case P_ZHONG:
        return 1;
    default:
        break;
    }
    assert(false);
}

void hui_init(hu_unit_item *p, std::initializer_list<int>&& cards, UnitType type, UnitSubType subtype)
{
    int i = 0;    
    for(auto v : cards) {
        p->idx[i++] = v;
    }
    assert(i == hui_count(subtype));
    p->type = type;
    p->subtype = subtype;
}

hu_unit_item hui_P(int cardidx)
 {
    hu_unit_item p;
    p.type = UIT_P;
    if(cardidx >= HAND_CARDIDX_LAY_FENGSTART && cardidx != JOKER_INDEX) {
        p.subtype = P_FB;
    } else {
        auto v = get_card_value_byidx(cardidx);
        if(v == 1 || v == 9) {
            p.subtype = P_19;
        } else if(v == 2 || v == 8) {
            p.subtype = P_28;
        } else {
            p.subtype = P_ZHONG;
        }
    }
    p.idx[0] = cardidx;
    return p;
}

hu_unit_item hui_M_3JK()
{
    hu_unit_item m;
    m.idx[0] = JOKER_INDEX;
    m.idx[1] = JOKER_INDEX;
    m.idx[2] = JOKER_INDEX;
    m.type = UIT_M;
    m.subtype = M_3JK;
    return m;
}

hu_unit_item hui_J_2JK()
{
    hu_unit_item m;
    m.idx[0] = JOKER_INDEX;
    m.idx[1] = JOKER_INDEX;
    m.type = UIT_J;
    m.subtype = J_2JK;
    return m;
}

hu_unit_item hui_M_2JK(int cardidx)
 {
    hu_unit_item m;
    m.idx[0] = cardidx;
    m.idx[1] = JOKER_INDEX;
    m.idx[2] = JOKER_INDEX;
    m.type = UIT_M;
    m.subtype = M_2JK;
    return m;
}

int hui_D2M(hu_unit_item *d, hu_unit_item ms[2])
{
    int res = 0;
    zero_struct(ms[0]);zero_struct(ms[1]);
    if (d->subtype == D_LIAN_BIAN) {
        auto v = get_card_value_byidx(d->idx[0]);
        if (v == 1) {
            // 1,2 边张
            auto& i = ms[res++];
            i.idx[0] = d->idx[0];
            i.idx[1] = d->idx[1];
            i.idx[2] = -(d->idx[1] + 1);
            i.type = UIT_M;
            i.subtype = M_BIAN;
        } else {
            // 8,9边张
            auto& i = ms[res++];
            i.idx[0] = -(d->idx[1] - 1);
            i.idx[1] = d->idx[0];
            i.idx[2] = d->idx[1];
            i.type = UIT_M;
            i.subtype = M_BIAN;
        }
    } else if (d->subtype == D_LIAN_ZHONG) {
        auto v = get_card_value_byidx(d->idx[0]);
        {
            auto subtype = M_ZHONG;
            if (v == 2) {
                subtype = M_BIAN;
            }
            auto& i = ms[res++];
            i.idx[0] = -(d->idx[1] - 1);
            i.idx[1] = d->idx[0];
            i.idx[2] = d->idx[1];
            i.type = UIT_M;
            i.subtype = subtype;
        }
        {
            auto subtype = M_ZHONG;
            if (v == 7) {
                subtype = M_BIAN;
            }
            auto& i = ms[res++];
            i.idx[0] = d->idx[0];
            i.idx[1] = d->idx[1];
            i.idx[2] = -(d->idx[1] + 1);
            i.type = UIT_M;
            i.subtype = subtype;
        }
    } else if (d->subtype == D_KAN_BIAN || d->subtype == D_KAN_ZHONG) {
        UnitSubType subtype;
        if(d->subtype == D_KAN_BIAN) {
            subtype = M_BIAN;
        } else {
            subtype = M_ZHONG;
        }

        auto& i = ms[res++];
        i.idx[0] = d->idx[0];
        i.idx[1] = -(d->idx[0] + 1);
        i.idx[2] = d->idx[1];
        i.type = UIT_M;
        i.subtype = subtype;
    } else {
        assert(false);
    }
    return res;
}

c_static_array<hu_unit_item, 2> hui_D2M_1(hu_unit_item *d)
{
    c_static_array<hu_unit_item, 2> res;
    zero_struct(res);
    res.count = hui_D2M(d,res.pat(0));
    return res;
}

hu_unit_item hui_J2M(hu_unit_item *j)
{
    hu_unit_item m;
    m.idx[0] = j->idx[0];
    m.idx[1] = j->idx[0];
    m.idx[1] = -j->idx[0];
    m.type = UIT_M;
    m.subtype = M_KEZI;
    return m;
}

hu_unit_item hui_P2J(hu_unit_item *p)
{
    hu_unit_item j;
    j.type = UIT_J;
    j.idx[0] = p->idx[0];
    j.idx[1] = -p->idx[0];
    if(p->subtype == P_28) {
        j.subtype = J_28;
    } else if(p->subtype == P_19) {
        j.subtype = J_19;
    } else if (p->subtype == P_FB) {
        j.subtype = J_FB;
    } else if(p->subtype == P_ZHONG){
        j.subtype = J_ZHONG;
    } else {
        assert(false);
    }
    return j;
}

hu_unit_item hui_P2J_1(hu_unit_item &&p)
{
    return hui_P2J(&p);
}

hu_unit_item hui_P2M(hu_unit_item *p)
{
    hu_unit_item m;
    m.idx[0] = p->idx[0];
    m.idx[1] = JOKER_INDEX;
    m.idx[1] = JOKER_INDEX;
    m.type = UIT_M;
    m.subtype = M_2JK;
    return m;
}

#define MAKE_MDJP_TABLE(m,d,j,p) ChecAndGet<m,d,j,p,jk>()
template<int M,int D,int J,int P, int JK>
static constexpr int ChecAndGet() {
    static_assert((M*3 + 2*D + 2*J + P + JK) == HAND_CARDS_COUNT);
    int res = M;
    res = res * 10 + D;
    res = res * 10 + J; 
    res = res * 10 + P;
    return res;
}

int calc_mdjp(hand_card_units *p)
{
    int res = p->M.count;
    res = res * 10 + p->D.count;
    res = res * 10 + p->J.count;
    res = res * 10 + p->P.count;
    return res;
}

int get_hu_mdjp_des_4m1j(int joker, int *&des)
{
    if(joker == 0) {
        constexpr int jk = 0;
        static int g_des[] = {
            MAKE_MDJP_TABLE(4,0,1,0)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if (joker == 1) {
        constexpr int jk = 1;
        static int g_des[] = {
            MAKE_MDJP_TABLE(4,0,0,1),MAKE_MDJP_TABLE(3,1,1,0),MAKE_MDJP_TABLE(3,0,2,0)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if(joker == 2) {
        constexpr int jk = 2;
        static int g_des[] = {
            MAKE_MDJP_TABLE(4,0,0,0),MAKE_MDJP_TABLE(3,0,1,1),
            MAKE_MDJP_TABLE(3,1,0,1),MAKE_MDJP_TABLE(2,2,1,0),
            MAKE_MDJP_TABLE(2,1,2,0),MAKE_MDJP_TABLE(2,0,3,0),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 3) {
        constexpr int jk = 3;
        static int g_des[] = {
            MAKE_MDJP_TABLE(3,0,1,0),MAKE_MDJP_TABLE(3,1,0,0),
            MAKE_MDJP_TABLE(3,0,0,2),MAKE_MDJP_TABLE(2,2,0,1),
            MAKE_MDJP_TABLE(2,1,1,1),MAKE_MDJP_TABLE(2,0,2,1),
            MAKE_MDJP_TABLE(1,0,4,0),MAKE_MDJP_TABLE(1,1,3,0),
            MAKE_MDJP_TABLE(1,2,2,0),MAKE_MDJP_TABLE(1,3,1,0),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 4) {
        constexpr int jk = 4;
        static int g_des[] = {
            MAKE_MDJP_TABLE(3,0,0,1),MAKE_MDJP_TABLE(2,2,0,0),
            MAKE_MDJP_TABLE(2,1,1,0),MAKE_MDJP_TABLE(2,1,0,2),
            MAKE_MDJP_TABLE(2,0,2,0),MAKE_MDJP_TABLE(2,0,1,2),
            MAKE_MDJP_TABLE(1,3,0,1),MAKE_MDJP_TABLE(1,2,1,1),
            MAKE_MDJP_TABLE(1,1,2,1),MAKE_MDJP_TABLE(1,0,3,1),
            MAKE_MDJP_TABLE(0,4,1,0),MAKE_MDJP_TABLE(0,3,2,0),
            MAKE_MDJP_TABLE(0,2,3,0),MAKE_MDJP_TABLE(0,1,4,0),
            MAKE_MDJP_TABLE(0,0,5,0),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 5) {
        constexpr int jk = 5;
        static int g_des[] = {
            MAKE_MDJP_TABLE(3,0,0,0),MAKE_MDJP_TABLE(2,1,0,1),
            MAKE_MDJP_TABLE(2,0,1,1),MAKE_MDJP_TABLE(2,0,0,3),
            MAKE_MDJP_TABLE(1,3,0,0),MAKE_MDJP_TABLE(1,2,1,0),
            MAKE_MDJP_TABLE(1,2,0,2),MAKE_MDJP_TABLE(1,1,2,0),
            MAKE_MDJP_TABLE(1,1,1,2),MAKE_MDJP_TABLE(1,0,3,0),
            MAKE_MDJP_TABLE(1,0,2,2),MAKE_MDJP_TABLE(0,0,4,1),
            MAKE_MDJP_TABLE(0,4,0,1),MAKE_MDJP_TABLE(0,3,1,1),
            MAKE_MDJP_TABLE(0,2,2,1),MAKE_MDJP_TABLE(0,1,3,1),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 6) {
        constexpr int jk = 6;
        static int g_des[] = {
            MAKE_MDJP_TABLE(2,1,0,0),MAKE_MDJP_TABLE(2,0,1,0),
            MAKE_MDJP_TABLE(2, 0, 0, 2),MAKE_MDJP_TABLE(1, 2, 0, 1),
            MAKE_MDJP_TABLE(1, 1, 1, 1),MAKE_MDJP_TABLE(1, 1, 0, 3),
            MAKE_MDJP_TABLE(1, 0, 2, 1),MAKE_MDJP_TABLE(1, 0, 1, 3),
            MAKE_MDJP_TABLE(0, 4, 0, 0),MAKE_MDJP_TABLE(0, 3, 1, 0),
            MAKE_MDJP_TABLE(0, 3, 0, 2),MAKE_MDJP_TABLE(0, 2, 2, 0),
            MAKE_MDJP_TABLE(0, 2, 1, 2),MAKE_MDJP_TABLE(0, 1, 3, 0),
            MAKE_MDJP_TABLE(0, 1, 2, 2),MAKE_MDJP_TABLE(0, 0, 4, 0),
            MAKE_MDJP_TABLE(0, 0, 3, 2),
        };
        des = &g_des[0];
        return std::size(g_des);
    }

    assert(false);
    return -1;
}

int get_hu_mdjp_des_7j(int joker, int*& des)
{
    if(joker == 0) {
        constexpr int jk = 0;
        static int g_des[] = {
            MAKE_MDJP_TABLE(0,0,7,0)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if (joker == 1) {
        constexpr int jk = 1;
        static int g_des[] = {
            MAKE_MDJP_TABLE(0,0,6,1),
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if(joker == 2) {
        constexpr int jk = 2;
        static int g_des[] = {
            MAKE_MDJP_TABLE(0,0,6,0),MAKE_MDJP_TABLE(0,0,5,2),
            MAKE_MDJP_TABLE(0,1,5,0),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 3) {
        constexpr int jk = 3;
        static int g_des[] = {
            MAKE_MDJP_TABLE(0,0,5,1),MAKE_MDJP_TABLE(0,1,4,1),
            MAKE_MDJP_TABLE(0,0,4,3),MAKE_MDJP_TABLE(1,0,4,0),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 4) {
        constexpr int jk = 4;
        static int g_des[] = {
            MAKE_MDJP_TABLE(1,0,3,1),MAKE_MDJP_TABLE(0,2,3,0),
            MAKE_MDJP_TABLE(0,1,4,0),MAKE_MDJP_TABLE(0,0,5,0),
            MAKE_MDJP_TABLE(0,0,4,2),MAKE_MDJP_TABLE(0,0,3,4),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 5) {
        constexpr int jk = 5;
        static int g_des[] = {
            MAKE_MDJP_TABLE(1,1,2,0),MAKE_MDJP_TABLE(1,0,3,0),
            MAKE_MDJP_TABLE(1,0,2,2),MAKE_MDJP_TABLE(0,2,2,1),
            MAKE_MDJP_TABLE(0,1,3,1),MAKE_MDJP_TABLE(0,1,2,3),
            MAKE_MDJP_TABLE(0,0,4,1),MAKE_MDJP_TABLE(0,0,3,3),
            MAKE_MDJP_TABLE(0,0,2,5)
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 6) {
        constexpr int jk = 6;
        static int g_des[] = {
            MAKE_MDJP_TABLE(2, 0, 1, 0),MAKE_MDJP_TABLE(1, 1, 1, 1),
            MAKE_MDJP_TABLE(1, 0, 2, 1),MAKE_MDJP_TABLE(1, 0, 1, 3),
            MAKE_MDJP_TABLE(0, 3, 1, 0),MAKE_MDJP_TABLE(0, 2, 2, 0),
            MAKE_MDJP_TABLE(0, 2, 1, 2),MAKE_MDJP_TABLE(0, 1, 2, 2),
            MAKE_MDJP_TABLE(0, 1, 3, 0),MAKE_MDJP_TABLE(0, 1, 1, 4),
            MAKE_MDJP_TABLE(0, 0, 4, 0),MAKE_MDJP_TABLE(0, 0, 3, 2),
            MAKE_MDJP_TABLE(0, 0, 2, 4),MAKE_MDJP_TABLE(0, 0, 1, 6),
        };
        des = &g_des[0];
        return std::size(g_des);
    }

    assert(false);
    return -1;
}

void mix_hu_mdjp_travel(int joker, hand_card_units* p, void*ud, bool(*func)(hu_card_units* u,void* ud))
{
    int M = p->M.count;
    int D = p->D.count;
    int J = p->J.count;
    int P = p->P.count;
    int mdjp = calc_mdjp(p);
    hu_card_units u;
#define CHECK_MDJP(m,d,j,p) MAKE_MDJP_TABLE(m,d,j,p)==mdjp
#define QCALL()    if(func(&u,ud)) {return;}
    if(joker == 0) {
        constexpr int jk = 0;
        if(CHECK_MDJP(4,0,1,0)) {
            zero_struct(u);
            u.M = p->M;
            u.J = p->J;
            QCALL();
        }
        if(CHECK_MDJP(0,0,7,0)) {
            zero_struct(u);
            u.J = p->J;
            QCALL();
        }
    } else if(joker == 1) {
        constexpr int jk = 1;
        if(CHECK_MDJP(4,0,0,1)) {
            // P + JK = J
            zero_struct(u);
            u.M = p->M;
            *u.J.grap() = hui_P2J(&p->P[0]);
            QCALL();
        }
        if(CHECK_MDJP(3,1,1,0)) { 
            // D+JK=M
            hu_unit_item tmp_m[2];
            int count = hui_D2M(&p->D[0], tmp_m);
            for(int i = 0; i < count; ++i) {
                u.M = p->M;
                u.J = p->J;
                *u.M.grap() = tmp_m[i];
                QCALL();
            }
        }
        if(CHECK_MDJP(3,0,2,0)) { 
            // J+JK=M
            for(int i = 0; i < p->J.count; ++i) {
                u.M = p->M;
                *u.J.grap() = p->J[(i+1)%2];
                *u.M.grap() = hui_J2M(&p->J[i]);
                QCALL();
            }
        }
        if(CHECK_MDJP(0,0,6,1)) {
            // P+JK=J
            u.J = p->J;
            *u.J.grap() = hui_P2J(&p->P[0]);
            QCALL();
        }
    } else if(joker == 2) {
        constexpr int jk = 2;
        if(CHECK_MDJP(4,0,0,0)) {
            // 2JK=J
            u.M = p->M;
            *u.J.grap() = hui_J_2JK();
            QCALL();
        }
        if(CHECK_MDJP(3,0,1,1)) {
            // 2JK+P=M
            zero_struct(u);
            u.M = p->M;
            *u.M.grap() = hui_P2M(&p->P[0]);
            QCALL();

            // J+JK=M, P+JK=J
            zero_struct(u);
            u.M = p->M;
            *u.M.grap() = hui_J2M(&p->J[0]);
            *u.J.grap() = hui_P2J(&p->P[0]);
            QCALL();
        }
        if(CHECK_MDJP(3,1,0,1)) {
            // D+JK=M,P+JK=J
            auto j = hui_P2J(&p->P[0]);
            hu_unit_item tmp_m[2];
            int count = hui_D2M(&p->D[0], tmp_m);
            for(int i = 0; i < count; ++i) {
                zero_struct(u);
                u.M = p->M;
                *u.M.grap() = tmp_m[i];
                *u.J.grap() = j;
                QCALL();
            }
        }
        if(CHECK_MDJP(2,1,2,0)) {
            // D+JK=M,J+JK=M
            hu_unit_item tmp_m[2];
            int count = hui_D2M(&p->D[0], tmp_m);
            for(int i = 0; i < count; ++i) {
                for(int j = 0; i < p->J.count; ++j) {
                    zero_struct(u);
                    u.M = p->M;
                    *u.M.grap() = tmp_m[i];
                    *u.M.grap() = hui_J2M(&p->J[j]);
                    *u.J.grap() = p->J[(j+1)%p->J.count];
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2, 2, 1, 0)) {
            // D+JK=M,D+JK=M
            hu_unit_item tmp_m1[2];
            hu_unit_item tmp_m2[2];
            int count1 = hui_D2M(&p->D[0], tmp_m1);
            int count2 = hui_D2M(&p->D[1], tmp_m2);
            for(int i = 0; i < count1; ++i) {
                for(int j = 0; j < count2; ++j) {
                    zero_struct(u);
                    u.M = p->M;
                    u.J = p->J;
                    *u.M.grap() = tmp_m1[i];
                    *u.M.grap() = tmp_m2[j];
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(0, 0, 6, 0)) {
            // 2JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_J_2JK());
            QCALL();
        }
        if(CHECK_MDJP(0,0,5,2)) {
            // P+JK=J,P+JK=J
            zero_struct(u);
            u.J = p->J;
            for(auto& t:p->P) {
                u.J.push(hui_P2J(&t));
            }
            QCALL();
        }
        if(CHECK_MDJP(0,1,5,0)) {
            //D=2P,2P+2JK=2J
            zero_struct(u);
            u.J = p->J;
            for(int i = 0; i < 2; ++i) {
                auto t = hui_P(p->D[0].idx[i]);
                u.J.push(hui_P2J(&t));
            }
            QCALL();
        }
    } else if(joker == 3) {
        constexpr int jk = 3;
        if(CHECK_MDJP(3,0,1,0)) {
            // 3JK=M
            zero_struct(u);
            u.M = p->M;
            u.J = p->J;
            u.M.push(hui_M_3JK());
            QCALL();

            // J+JK=M,2JK=J
            zero_struct(u);
            u.M = p->M;
            u.M.push(hui_J2M(&p->J[0]));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        if(CHECK_MDJP(3,1,0,0)) {
            // D+JK=M, 2JK=J
            hu_unit_item tmp_m[2];
            int count = hui_D2M(&p->D[0], tmp_m);
            for(int i = 0; i < count; ++i) {
                zero_struct(u);
                u.M = p->M;
                u.M.push(tmp_m[i]);
                u.J.push(hui_J_2JK());
                QCALL();
            }
        }
        if(CHECK_MDJP(3, 0, 0, 2)) {
            // P+2JK=M,P+JK=J
            for(int i = 0; i < 2; ++i) {
                zero_struct(u);
                u.M = p->M;
                u.J.push(hui_P2J(&p->P[i]));
                u.M.push(hui_M_2JK(p->P[(i+1)%2].idx[0]));
                QCALL();
            }
        }
        if(CHECK_MDJP(2,2,0,1)) {
            // 2D+2JK=2M,P+JK=J
            hu_unit_item tmp_m1[2];
            int count1 = hui_D2M(&p->D[0], tmp_m1);
            hu_unit_item tmp_m2[2];
            int count2 = hui_D2M(&p->D[1], tmp_m2);
            for(int i = 0; i < count1; ++i) {
                for(int j = 0; j < count2; ++j) {
                    zero_struct(u);
                    u.M = p->M;
                    u.J.push(hui_P2J(&p->P[0]));
                    u.M.push(tmp_m1[i]);
                    u.M.push(tmp_m2[j]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2,1,1,1)) {
            hu_unit_item tmp_m[2];
            int count = hui_D2M(p->D.pat(0), tmp_m);

            // D+JK=M,J+JK=M,P+JK=J
            for (size_t i = 0; i < count; i++)
            {
                zero_struct(u);
                u.M = p->M;
                u.M.push(tmp_m[i]);
                u.M.push(hui_J2M(p->J.pat(0)));
                u.J.push(hui_P2J(p->P.pat(0)));
                QCALL();
            }

            // D+JK=M,P+2JK=M
            for (size_t i = 0; i < count; i++)
            {
                zero_struct(u);
                u.M = p->M;
                u.J = p->J;
                u.M.push(tmp_m[i]);
                u.M.push(hui_M_2JK(p->P[0].idx[0]));
                QCALL();
            }
        }
        if(CHECK_MDJP(2,0,2,1)) {
            // 2JK+2JK=2M,P+JK=J
            zero_struct(u);
            u.M = p->M;
            for(auto& t:p->J) {
                u.M.push(hui_J2M(&t));
            }
            u.J.push(hui_P2J(p->P.pat(0)));
            QCALL();
            // P+2JK=M,J+JK=M
            for(int i = 0; i < 2; ++i) {
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.pat(0)));
                u.M.push(hui_J2M(p->J.pat(i)));
                u.J.push(p->J[(i+1)%2]);
                QCALL();
            }
        }
        if(CHECK_MDJP(1,0,4,0)) {
            // 3J+3JK=3M
            hu_unit_item tmp_m[] = {
                hui_J2M(p->J.pat(0)),
                hui_J2M(p->J.pat(1)),
                hui_J2M(p->J.pat(2)),
                hui_J2M(p->J.pat(3))
            };

            for(int i = 0; i < std::size(tmp_m); ++i) {
                zero_struct(u);
                u.M = p->M;
                u.J.push(p->J[i]);
                for(int j = 0; j < std::size(tmp_m); ++j) {
                    if(i == j) continue;
                    u.M.push(tmp_m[j]);
                }
                QCALL();
            }
        }
        if(CHECK_MDJP(1,1,3,0)) {
            // D+JK=M,2J+2JK=M
            hu_unit_item tmp_m[2];
            int count = hui_D2M(p->D.pat(0), tmp_m);
            hu_unit_item tmp_j2m[] = {
                hui_J2M(p->P.pat(0)),
                hui_J2M(p->P.pat(1)),
                hui_J2M(p->P.pat(2)),
            };
            for(int i = 0; i < count; ++i) {
                for(int j = 0; j < std::size(tmp_j2m); ++j) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(tmp_m[i]);
                    u.J.push(p->J[j]);
                    for(int m = 0; m < std::size(tmp_j2m); ++m) {
                        if(m == j) continue;
                        u.M.push(tmp_j2m[m]);
                    }
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1,2,2,0)) {
            // 2D+2JK=2M,J+JK=M
            hu_unit_item tmp_m1[2];
            int count1 = hui_D2M(p->D.pat(0), tmp_m1);
            hu_unit_item tmp_m2[2];
            int count2 = hui_D2M(p->D.pat(1), tmp_m2);
            for(int i = 0; i < count1; ++i) {
                for(int j = 0; j < count2; ++j) {
                    for(int m = 0; m < 2; ++m) {
                        zero_struct(u);
                        u.M = p->M;
                        u.J.push(p->J[m]);
                        u.M.push(tmp_m1[i]);
                        u.M.push(tmp_m2[j]);
                        u.M.push(hui_J2M(p->J.pat((m+1)%2)));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1,3,1,0)) {
            //3D+3JK=3M
            auto dm1 = hui_D2M_1(p->D.pat(0));
            auto dm2 = hui_D2M_1(p->D.pat(1));
            auto dm3 = hui_D2M_1(p->D.pat(2));
            for(auto& m1:dm1) {
                for(auto& m2: dm2) {
                    for(auto& m3: dm3) {
                        zero_struct(u);
                        u.M = p->M;
                        u.J = p->J;
                        u.M.push(m1);
                        u.M.push(m2);
                        u.M.push(m3);
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(0,0,5,1)) {
            // P+JK=J,2JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_J_2JK());
            u.J.push(hui_P2J(p->P.pat(0)));
            QCALL();
        }
        if(CHECK_MDJP(0,1,4,1)) {
            // D=2P,2P+2JK=2J,3P+3JK=3J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[1])));
            QCALL();
        }
        if(CHECK_MDJP(0,0,4,3)) {
            // 3P+3JK=3J
            zero_struct(u);
            u.J = p->J;
            for(auto&t:p->P) {
                u.J.push(hui_P2J(&t));
            }
            QCALL();
        }
        if(CHECK_MDJP(1,0,4,0)) {
            // M=3P,3P+3JK=4J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[2])));
            QCALL();
        }
    } else if(joker == 4) {
        constexpr int jk = 4;
        if(CHECK_MDJP(3, 0, 0, 1)) {
            {
                // P+JK=J,3JK=M
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_M_3JK());
                u.J.push(hui_P2J(p->P.pat(0)));
                QCALL();
            }
            {
                // P+2JK=M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.pat(0)));
                u.J.push(hui_J_2JK());
                QCALL();
            }
        }
        if(CHECK_MDJP(2, 2, 0, 0)) {
            // 2D+2JK=2M,2JK=J
            auto d2m1 = hui_D2M_1(p->D.pat(0));
            auto d2m2 = hui_D2M_1(p->D.pat(1));
            for(auto& m1:d2m1) {
                for(auto& m2:d2m2) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m1);
                    u.M.push(m2);
                    u.J.push(hui_J_2JK());
                    QCALL();
                }
            }
        }
        if (CHECK_MDJP(2, 1, 1, 0)) {
            auto d2m = hui_D2M_1(p->D.pat(0));
            {
                // D+JK=M,3JK=M
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.J = p->J;
                    u.M.push(m);
                    u.M.push(hui_M_3JK());
                    QCALL();
                }
            }
            {
                // D+JK=M,J+JK=M,2JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.J.push(hui_J_2JK());
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2, 1, 0, 2)) {
            // D+JK=M,P+2JK=M,P+JK=J
            auto d2m = hui_D2M_1(p->D.pat(0));
            for(auto& m:d2m) {
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_P2M(p->P.pat(i)));
                    u.J.push(hui_P2J(p->P.pat(1-i)));
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2, 0, 2, 0)) {
            {
                // 2J+2JK=2M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // J+JK=M,3JK=M
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.pat(i)));
                    u.M.push(hui_M_3JK());
                    u.J.push(p->J[1-i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2, 0, 1, 2)) {
            {
                // J+JK=M,P+2JK=M,P+JK=J
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.begin()));
                    u.M.push(hui_P2M(p->P.pat(i)));
                    u.J.push(hui_P2J(p->P.pat(1-i)));
                    QCALL();
                }
            }
            {
                // 2P+4JK=2M
                zero_struct(u);
                u.M = p->M;
                u.J = p->J;
                u.M.push(hui_P2M(p->P.pat(0)));
                u.M.push(hui_P2M(p->P.pat(1)));
                QCALL();
            }
        }
        if(CHECK_MDJP(1, 3, 0, 1)) {
            // 3D+3JK=3M,P+JK=J
            auto d2m0 = hui_D2M_1(p->D.pat(0));
            auto d2m1 = hui_D2M_1(p->D.pat(1));
            auto d2m2 = hui_D2M_1(p->D.pat(2));
            for(auto& m0 : d2m0) {
                for(auto& m1: d2m1) {
                    for(auto& m2: d2m2) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(m2);
                        u.J.push(hui_P2J(p->P.begin()));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 2, 1, 1)) {
            auto d2m0 = hui_D2M_1(p->D.pat(0));
            auto d2m1 = hui_D2M_1(p->D.pat(1));
            {
                // 2D+2JK=2M,P+2JK=M
                for(auto& m0: d2m0) {
                    for(auto& m1:d2m1) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_P2M(p->P.begin()));
                        u.J = p->J;
                        QCALL();
                    }
                }
            }
            {
                // 2D+2JK=2M,J+JK=M,P+JK=J
                for(auto& m0: d2m0) {
                    for(auto& m1:d2m1) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_J2M(p->J.begin()));
                        u.J.push(hui_P2J(p->P.begin()));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 1, 2, 1)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,J+JK=M,P+2JK=M
                for(auto&m:d2m) {
                    for(int i = 0; i < 2; ++i) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m);
                        u.M.push(hui_P2M(p->P.begin()));
                        u.M.push(hui_J2M(p->J.pat(i)));
                        u.J.push(p->J[1-i]);
                        QCALL();
                    }
                }
            }
            {
                // D+JK=M,2J+2JK=M,P+JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(m);
                    u.J.push(hui_P2J(p->P.begin()));
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1, 0, 3, 1)) {
            {
                // 2J+2JK=2M,P+2JK=M
                for(int i = 0; i < 3; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_P2M(p->P.begin()));
                    u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                    u.J.push(p->J[i]);
                    QCALL();
                }
            }
            {
                // 3J+3JK=3M,P+JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_J2M(p->J.pat(2)));
                u.J.push(hui_P2J(p->P.begin()));
                QCALL();
            }
        }
        if(CHECK_MDJP(0, 4, 1, 0)) {
            // 4D+4JK=4M
            for(auto&m0 : hui_D2M_1(p->D.pat(0))) {
                for(auto&m1 : hui_D2M_1(p->D.pat(1))) {
                    for(auto&m2 : hui_D2M_1(p->D.pat(2))) {
                        for(auto&m3: hui_D2M_1(p->D.pat(3))) {
                            zero_struct(u);
                            u.J = p->J;
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(m3);
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 3, 2, 0)) {
            // 3D+3JK=M,J+JK=M
            for(auto&m0 : hui_D2M_1(p->D.pat(0))) {
                for(auto&m1 : hui_D2M_1(p->D.pat(1))) {
                    for(auto&m2 : hui_D2M_1(p->D.pat(2))) {
                        for(int i = 0; i < 2; ++i) {
                            zero_struct(u);
                            u.J.push(p->J[i]);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_J2M(p->J.pat(1-i)));
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 2, 3, 0)) {
            // 2D+2JK=2M,2J+2JK=2M
            for(auto&m0 : hui_D2M_1(p->D.pat(0))) {
                for(auto&m1 : hui_D2M_1(p->D.pat(1))) {
                    for(int i = 0; i < 3; ++i) {
                        zero_struct(u);
                        u.J.push(p->J[i]);
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                        u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 1, 4, 0)) {
            // D+JK=M,3J+3JK=3M
            for(auto& m:hui_D2M_1(p->D.begin())) {
                for(int i = 0; i < 4; ++i) {
                    zero_struct(u);
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.pat((i+1)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+3)%4)));
                    u.J.push(p->J[i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(0, 0, 5, 0)) {
            // 4J+4JK=4M
            for(int i = 0; i < 5; ++i) {
                zero_struct(u);
                u.J.push(p->J[i]);
                u.M.push(hui_J2M(p->J.pat((i+1)%4)));
                u.M.push(hui_J2M(p->J.pat((i+2)%4)));
                u.M.push(hui_J2M(p->J.pat((i+3)%4)));
                u.M.push(hui_J2M(p->J.pat((i+4)%4)));
                QCALL();
            }
        }

        // 7J
        if(CHECK_MDJP(1, 0, 3, 1)) {
            // M+3JK=3J,P+JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J(p->P.begin()));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[2])));
            QCALL();
        }
        // 2D+3J
        if(CHECK_MDJP(0, 2, 3, 0)) {
            // 2D+4JK=4J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[2])));
            u.J.push(hui_P2J_1(hui_P(p->D[1].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[1].idx[2])));
            QCALL();
        }
        // 1D+4J
        if(CHECK_MDJP(0, 1, 4, 0)) {
            // D+2JK=2J,2JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[2])));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 5J
        if(CHECK_MDJP(0, 0, 5, 0)) {
            // 4JK=2J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 4J+2P
        if(CHECK_MDJP(0, 0, 4, 2)) {
            // 2P+2JK=2J,2JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 3J+4P
        if(CHECK_MDJP(0, 0, 3, 4)) {
            // 4P+4JK=4J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_P2J(p->P.pat(2)));
            u.J.push(hui_P2J(p->P.pat(3)));
            QCALL();
        }
    } else if(joker == 5) {
        constexpr int jk = 5;
        if(CHECK_MDJP(3, 0, 0, 0)) {
            // 3JK=M,2JK=J
            zero_struct(u);
            u.M = p->M;
            u.M.push(hui_M_3JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        if(CHECK_MDJP(2, 1, 0, 1)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,P+2JK=M,2JK=J
                for(auto& m: d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_P2M(p->P.begin()));
                    u.J.push(hui_J_2JK());
                    QCALL();
                }
            }
            {
                // D+JK=M,3JK=M,P+JK=J
                for(auto& m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_M_3JK());
                    u.J.push(hui_P2J(p->P.begin()));
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(2, 0, 1, 1)) {
            {
                // J+JK=M,P+2JK=M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.begin()));
                u.M.push(hui_P2M(p->P.begin()));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // J+JK=M,3JK=M,P+JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.begin()));
                u.M.push(hui_M_3JK());
                u.J.push(hui_P2J(p->P.begin()));
                QCALL();
            }
            {
                // P+2JK=M,3JK=M
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.begin()));
                u.M.push(hui_M_3JK());
                u.J.push(p->J[0]);
                QCALL();
            }
        }
        if(CHECK_MDJP(2, 0, 0, 3)) {
            // 2P+4JK=2M,P+JK=J
            for(int i = 0; i < 3; ++i) {
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.pat((i+1)%3)));
                u.M.push(hui_P2M(p->P.pat((i+2)%3)));
                u.J.push(hui_P2J(p->P.pat(i)));
                QCALL();
            }
        }
        if(CHECK_MDJP(1, 3, 0, 0)) {
            // 3D+3JK=3M,2JK=J
            zero_struct(u);
            u.M = p->M;
            for(auto&m0:hui_D2M_1(p->D.pat(0))) {
                for(auto&m1:hui_D2M_1(p->D.pat(1))) {
                    for(auto&m2:hui_D2M_1(p->D.pat(2))) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(m2);
                        u.J.push(hui_J_2JK());
                        QCALL();
                    }
                }   
            }
        }
        if(CHECK_MDJP(1, 2, 1, 0)) {
            auto d2m0 = hui_D2M_1(p->D.pat(0));
            auto d2m1 = hui_D2M_1(p->D.pat(1));
            {
                // 2D+2JK=2M,3JK=M
                for(auto&m0:d2m0) {
                    for(auto&m1:d2m1) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_M_3JK());
                        u.J.push(p->J[0]);
                        QCALL();
                    }
                }
            }
            {
                // 2D+2JK=2M,J+JK=M,2JK=J
                for(auto&m0:d2m0) {
                    for(auto&m1:d2m1) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_J2M(p->J.begin()));
                        u.J.push(hui_J_2JK());
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 2, 0, 2)) {
            // 2D+2JK=2M,P+2JK=M,P+JK=J
            for(auto&m0:hui_D2M_1(p->D.pat(0))) {
                for(auto&m1:hui_D2M_1(p->D.pat(1))) {
                    for(int i = 0; i < 2; ++i) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_P2M(p->P.pat(i)));
                        u.J.push(hui_P2J(p->P.pat(1-i)));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 1, 2, 0)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,2J+2JK=2M,2JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(hui_J_2JK());
                    QCALL();
                }
            }
            {
                // D+JK=M,J+JK=M,3JK=M
                for(auto&m:d2m) {
                    for(int i = 0; i < 2; ++i) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m);
                        u.M.push(hui_J2M(p->J.pat(i)));
                        u.M.push(hui_M_3JK());
                        u.J.push(p->J[1-i]);
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 1, 1, 2)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=2M,2P+4JK=2M
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_P2M(p->P.pat(0)));
                    u.M.push(hui_P2M(p->P.pat(1)));
                    u.J.push(p->J[0]);
                    QCALL();
                }
            }
            {
                // D+JK=2M,J+JK=M,P+2JK=M,P+JK=J
                for(auto&m:d2m) {
                    for(int i = 0;i < 2; ++i) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m);
                        u.M.push(hui_J2M(p->J.begin()));
                        u.M.push(hui_P2M(p->P.pat(i)));
                        u.J.push(hui_P2J(p->P.pat(1-i)));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 0, 3, 0)) {
            {
                // 3J+3JK=3M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_J2M(p->J.pat(2)));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // 2J+2JK=2M,3JK=M
                for(int i = 0; i < 3; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_M_3JK());
                    u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                    u.J.push(p->J[i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1, 0, 2, 2)) {
            {
                // 2J+2JK=2M,P+2JK=M,P+JK=J
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(hui_P2M(p->P.pat(i)));
                    u.J.push(hui_P2J(p->P.pat(1-i)));
                    QCALL();
                }
            }
            {
                // J+JK=M,2P+4JK=2M
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.pat(i)));
                    u.M.push(hui_P2M(p->P.pat(0)));
                    u.M.push(hui_P2M(p->P.pat(1)));
                    u.J.push(p->J[1-i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(0, 0, 4, 1)) {
            {
                // 4J+4JK=4M,P+JK=J
                zero_struct(u);
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_J2M(p->J.pat(2)));
                u.M.push(hui_J2M(p->J.pat(3)));
                u.J.push(hui_P2J(p->P.begin()));
                QCALL();
            }
            {
                // 3J+3JK=3M,P+2JK=M
                for(int i = 0; i < 4; ++i) {
                    zero_struct(u);
                    u.J.push(p->J[i]);
                    u.M.push(hui_J2M(p->J.pat((i+1)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+3)%4)));
                    u.M.push(hui_P2M(p->P.begin()));
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(0, 4, 0, 1)) {
            // 4D+4JK=4M,P+JK=J
            zero_struct(u);
            for(auto&m0:hui_D2M_1(p->D.pat(0))) {
                for(auto&m1:hui_D2M_1(p->D.pat(1))) {
                    for(auto&m2:hui_D2M_1(p->D.pat(2))) {
                        for(auto&m3:hui_D2M_1(p->D.pat(3))) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(m3);
                            u.J.push(hui_P2J(p->P.begin()));
                            QCALL();
                        }
                    }
                }   
            }
        }
        if(CHECK_MDJP(0, 3, 1, 1)) {
            c_static_array<hu_unit_item, 2> d2m[] = {
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1)),hui_D2M_1(p->D.pat(2)),
            };
            {
                // 3D+3JK=3M,J+JK=M,P+JK=J
                for(auto&m0:d2m[0]) {
                    for(auto&m1:d2m[1]) {
                        for(auto&m2:d2m[2]) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_J2M(p->J.begin()));
                            u.J.push(hui_P2J(p->P.begin()));
                            QCALL();
                        }
                    }   
                }
            }
            {
                // 3D+3JK=3M,P+2JK=M
                for(auto&m0:d2m[0]) {
                    for(auto&m1:d2m[1]) {
                        for(auto&m2:d2m[2]) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_P2M(p->P.begin()));
                            u.J.push(p->J[0]);
                            QCALL();
                        }
                    }   
                }
            }
        }
        if(CHECK_MDJP(0, 2, 2, 1)) {
            c_static_array<hu_unit_item, 2> d2m[] = {
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1)),
            };
            {
                // 2D+2JK=2M,2J+2JK=2M,P+JK=J
                for(auto&m0:d2m[0]) {
                    for(auto&m1:d2m[1]) {
                        zero_struct(u);
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_J2M(p->J.pat(0)));
                        u.M.push(hui_J2M(p->J.pat(1)));
                        u.J.push(hui_P2J(p->P.begin()));
                        QCALL();
                    }   
                }
            }
            {
                // 2D+2JK=2M,J+JK=M,P+2JK=M
                for(auto&m0:d2m[0]) {
                    for(auto&m1:d2m[1]) {
                        
                        for(int i = 0; i < 2; ++i) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(hui_J2M(p->J.pat(i)));
                            u.M.push(hui_P2M(p->P.begin()));
                            u.J.push(p->J[1-i]);
                            QCALL();
                        }                        
                    }   
                }
            }
        }
        if(CHECK_MDJP(0, 1, 3, 1)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,3J+3JK=3M,P+JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(hui_J2M(p->J.pat(2)));
                    u.J.push(hui_P2J(p->P.begin()));
                    QCALL();
                }
            }
            {
                // D+JK=M,2J+2JK=2M,P+2JK=M
                for(auto&m:d2m) {
                    for(int i = 0; i < 3; ++i) {
                        zero_struct(u);
                        u.M.push(m);
                        u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                        u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                        u.M.push(hui_P2M(p->P.begin()));
                        u.J.push(p->J[i]);
                        QCALL();
                    }
                }
            }
        }
        // 7J
        // 1M+1D+2J
        if(CHECK_MDJP(1, 1, 2, 0)) {
            // M+3JK=3J,D+2JK=2J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[2])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[1])));
            QCALL();
        }
        // 1M+3J
        if(CHECK_MDJP(1, 0, 3, 0)) {
            // M+3JK=3M,2JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[2])));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 1M+2J+2P
        if(CHECK_MDJP(1, 0, 2, 2)) {
            // M+3JK=3M,2P+2JK=2J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M[0].idx[2])));
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            QCALL();
        }
        // 2D+2J+1P
        if(CHECK_MDJP(0, 2, 2, 1)) {
            // 2D+4JK=4J,P+JK=J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->D[1].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[1].idx[1])));
            u.J.push(hui_P2J(p->P.begin()));
            QCALL();
        }
        // 1D+3J+1P
        if(CHECK_MDJP(0, 1, 3, 1)) {
            // D+2JK=2J,P+JK=J,2JK=J
            zero_struct(u);
            u.J = p->J;      
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[1])));
            u.J.push(hui_P2J(p->P.begin()));
            u.J.push(hui_J_2JK());
            QCALL();     
        }
        // 1D+2J+3P
        if(CHECK_MDJP(0, 1, 2, 3)) {
            // D+2JK=2J,3P+3JK=3J
            zero_struct(u);
            u.J = p->J;   
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D[0].idx[1])));
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_P2J(p->P.pat(2)));
            QCALL();
        }
        // 4J+1P
        if(CHECK_MDJP(0, 0, 4, 1)) {
            // P+JK=J,4JK=2J
            zero_struct(u);
            u.J = p->J;   
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 3J+3P
        if(CHECK_MDJP(0, 0, 3, 3)) {
            // 3P+3JK=3J,2JK=J
            zero_struct(u);
            u.J = p->J;   
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_P2J(p->P.pat(2)));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 2J+5P
        if(CHECK_MDJP(0, 0, 2, 5)) {
            // 5P+5JK=5J
            zero_struct(u);
            u.J = p->J;   
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_P2J(p->P.pat(2)));
            u.J.push(hui_P2J(p->P.pat(3)));
            u.J.push(hui_P2J(p->P.pat(4)));
            QCALL();
        }
    } else if(joker == 6) {
        constexpr int jk = 6;
        if(CHECK_MDJP(2, 1, 0, 0)) {
            // D+JK=M,3JK=M,2JK=J
            for(auto&m:hui_D2M_1(p->D.begin())) {
                zero_struct(u);
                u.M = p->M;
                u.M.push(m);
                u.M.push(hui_M_3JK());
                u.J.push(hui_J_2JK());
                QCALL();
            }
        }
        if(CHECK_MDJP(2, 0, 1, 0)) {
            {
                // J+JK=M,3JK=M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.begin()));
                u.M.push(hui_M_3JK());
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // 6JK=2M
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_M_3JK());
                u.M.push(hui_M_3JK());
                u.J.push(p->J[0]);
                QCALL();
            }
        }
        if(CHECK_MDJP(2, 0, 0, 2)) {
            {
                // 2P+4JK=2M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.pat(0)));
                u.M.push(hui_P2M(p->P.pat(1)));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // P+2JK=M,3JK=M,P+JK=J
                for(int i = 0; i < 2;++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_P2M(p->P.pat(i)));
                    u.M.push(hui_M_3JK());
                    u.J.push(hui_P2J(p->P.pat(1-i)));
                    QCALL();
                }
            }

        }
        if(CHECK_MDJP(1, 2, 0, 1)) {
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1))
            );
            {
                // 2D+2JK=2M,P+2JK=M,2JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:get<1>(d2m)) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_P2M(p->P.begin()));
                        u.J.push(hui_J_2JK());
                        QCALL();
                    }
                }
            }
            {
                // 2D+2JK=2M,3JK=M,P+JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:get<1>(d2m)) {
                        zero_struct(u);
                        u.M = p->M;
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_M_3JK());
                        u.J.push(hui_P2J(p->P.begin()));
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(1, 1, 1, 1)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,J+JK=M,P+2JK=M,2JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.begin()));
                    u.M.push(hui_P2M(p->P.begin()));
                    u.J.push(hui_J_2JK());
                    QCALL();
                }
            }
            {
                // D+JK=M,J+JK=M,3JK+M,P+JK=J
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_J2M(p->J.begin()));
                    u.M.push(hui_M_3JK());
                    u.J.push(hui_P2J(p->P.begin()));
                    QCALL();
                }
            }
            {
                // D+JK=M,P+2JK=M,3JK=M
                for(auto&m:d2m) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_P2M(p->P.begin()));
                    u.M.push(hui_M_3JK());
                    u.J.push(p->J[0]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1, 1, 0, 3)) {
            // D+JK=M,2P+4JK=2M,P+JK=J
            for(auto&m:hui_D2M_1(p->D.begin())) {
                for(int i = 0; i < 3; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(m);
                    u.M.push(hui_P2M(p->P.pat((i+1)%3)));
                    u.M.push(hui_P2M(p->P.pat((i+2)%3)));
                    u.J.push(hui_P2J(p->P.pat(i)));
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1, 0, 2, 1)) {
            {
                // 2J+2JK=2M,P+2JK=M,2JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_P2J(p->P.begin()));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // 2J+2JK=2M,3JK=M,P+JK=J
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_M_3JK());
                u.J.push(hui_P2J(p->P.begin()));
                QCALL();
            }
            {
                // J+JK=M,3JK=M,P+2JK=M
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.pat(i)));
                    u.M.push(hui_M_3JK());
                    u.M.push(hui_P2M(p->P.begin()));
                    u.J.push(p->J[1-i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(1, 0, 1, 3)) {
            {
                // J+JK=M,2P+4JK=2M,P+JK=J
                for(int i = 0; i < 3; ++i) {
                    zero_struct(u);
                    u.M = p->M;
                    u.M.push(hui_J2M(p->J.begin()));
                    u.M.push(hui_P2M(p->P.pat((i+1)%3)));
                    u.M.push(hui_P2M(p->P.pat((i+2)%3)));
                    u.J.push(hui_P2J(p->P.pat(i)));
                    QCALL();
                }
            }
            {
                // 3P+6JK=3M
                zero_struct(u);
                u.M = p->M;
                u.M.push(hui_P2M(p->P.pat(0)));
                u.M.push(hui_P2M(p->P.pat(1)));
                u.M.push(hui_P2M(p->P.pat(2)));
                u.J.push(p->J[0]);
                QCALL();
            }
        }
        if(CHECK_MDJP(0, 4, 0, 0)) {
            // 4D+4JK=4M,2JK=J
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1)),
                hui_D2M_1(p->D.pat(2)),hui_D2M_1(p->D.pat(3))
            );
            for(auto&m0:std::get<0>(d2m)) {
                for(auto&m1:std::get<1>(d2m)) {
                    for(auto&m2:std::get<2>(d2m)) {
                        for(auto&m3:std::get<3>(d2m)) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(m3);
                            u.J.push(hui_J_2JK());
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 3, 1, 0)) {
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1)),
                hui_D2M_1(p->D.pat(2))
            );
            {
                // 3D+3JK=3M,J+JK=M,2JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        for(auto&m2:std::get<2>(d2m)) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_J2M(p->J.begin()));
                            u.J.push(hui_J_2JK());
                            QCALL();
                        }
                    }
                }
            }
            {
                // 3D+3JK=3M,3JK=M
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        for(auto&m2:std::get<2>(d2m)) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_M_3JK());
                            u.J.push(p->J[0]);
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 3, 0, 2)) {
            // 3D+3JK=3M,P+2JK=M,P+JK=J
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1)),
                hui_D2M_1(p->D.pat(2))
            );
            for(auto&m0:std::get<0>(d2m)) {
                for(auto&m1:std::get<1>(d2m)) {
                    for(auto&m2:std::get<2>(d2m)) {
                        for(int i = 0; i < 2; ++i) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(m2);
                            u.M.push(hui_P2M(p->P.pat(i)));
                            u.J.push(hui_P2J(p->P.pat(1-i)));
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 2, 2, 0)) {
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1))
            );
            {
                // 2D+2JK=2M,2J+2JK=2M,2JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        zero_struct(u);
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_J2M(p->J.pat(0)));
                        u.M.push(hui_J2M(p->J.pat(1)));
                        u.J.push(hui_J_2JK());
                        QCALL();
                    }
                }
            }
            {
                // 2D+2JK=2M,J+JK=M,3JK=M
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        for(int i = 0; i < 2; ++i) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(hui_J2M(p->J.pat(i)));
                            u.M.push(hui_M_3JK());
                            u.J.push(p->J[1-i]);
                            QCALL();
                        }
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 2, 1, 2)) {
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0)),hui_D2M_1(p->D.pat(1))
            );
            {
                // 2D+2JK=2M,J+JK=M,P+2JK=M,P+JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        for(int i = 0; i < 2; ++i) {
                            zero_struct(u);
                            u.M.push(m0);
                            u.M.push(m1);
                            u.M.push(hui_J2M(p->J.begin()));
                            u.M.push(hui_P2M(p->P.pat(i)));
                            u.J.push(hui_P2J(p->P.pat(1-i)));
                            QCALL();
                        }
                    }
                }
            }
            {
                // 2D+2JK=2M,2P+4JK=2M
                for(auto&m0:std::get<0>(d2m)) {
                    for(auto&m1:std::get<1>(d2m)) {
                        zero_struct(u);
                        u.M.push(m0);
                        u.M.push(m1);
                        u.M.push(hui_P2M(p->P.pat(0)));
                        u.M.push(hui_P2M(p->P.pat(1)));
                        u.J.push(p->J[0]);
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 1, 3, 0)) {
            auto d2m = std::make_tuple(
                hui_D2M_1(p->D.pat(0))
            );
            {
                // D+JK=M,3J+3JK=3M,2JK=J
                for(auto&m0:std::get<0>(d2m)) {
                    zero_struct(u);
                    u.M.push(m0);
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(hui_J2M(p->J.pat(2)));
                    u.J.push(hui_J_2JK());
                    QCALL();
                }
            }
            {
                // D+JK=M,2J+2JK=2M,3JK=M
                for(auto&m0:std::get<0>(d2m)) {
                    for(int i = 0; i < 3; ++i) {
                        zero_struct(u);
                        u.M.push(m0);
                        u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                        u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                        u.M.push(hui_M_3JK());
                        u.J.push(p->J[0]);
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 1, 2, 2)) {
            auto d2m = hui_D2M_1(p->D.begin());
            {
                // D+JK=M,2J+2JK=M,P+2JK=M,P+JK=J
                for(auto& m:d2m) {
                    for(int i = 0; i < 2; ++i) {
                        zero_struct(u);
                        u.M.push(m);
                        u.M.push(hui_J2M(p->J.pat(0)));
                        u.M.push(hui_J2M(p->J.pat(1)));
                        u.M.push(hui_P2M(p->P.pat(i)));
                        u.J.push(hui_P2J(p->P.pat(1-i)));
                        QCALL();
                    }
                }
            }
            {
                // D+JK=M,J+JK=M,2P+4JK=2M
                for(auto& m:d2m) {
                    for(int i = 0; i < 2; ++i) {
                        zero_struct(u);
                        u.M.push(m);
                        u.M.push(hui_J2M(p->J.pat(i)));
                        u.M.push(hui_P2M(p->P.pat(0)));
                        u.M.push(hui_P2M(p->P.pat(1)));
                        u.J.push(p->J[1-i]);
                        QCALL();
                    }
                }
            }
        }
        if(CHECK_MDJP(0, 0, 4, 0)) {
            {
                // 4J+4JK=4M,2JK=J
                zero_struct(u);
                u.M.push(hui_J2M(p->J.pat(0)));
                u.M.push(hui_J2M(p->J.pat(1)));
                u.M.push(hui_J2M(p->J.pat(2)));
                u.M.push(hui_J2M(p->J.pat(3)));
                u.J.push(hui_J_2JK());
                QCALL();
            }
            {
                // 3J+3JK=3M,3JK=M
                for(int i = 0; i < 4; ++i) {
                    zero_struct(u);
                    u.M.push(hui_J2M(p->J.pat((i+1)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%4)));
                    u.M.push(hui_J2M(p->J.pat((i+3)%4)));
                    u.M.push(hui_M_3JK());
                    u.J.push(p->J[i]);
                    QCALL();
                }
            }
        }
        if(CHECK_MDJP(0, 0, 3, 2)) {
            {
                // 3J+3JK=3M,P+2JK=M,P+JK=J
                for(int i = 0; i < 2; ++i) {
                    zero_struct(u);
                    u.M.push(hui_J2M(p->J.pat(0)));
                    u.M.push(hui_J2M(p->J.pat(1)));
                    u.M.push(hui_J2M(p->J.pat(2)));
                    u.M.push(hui_P2M(p->P.pat(i)));
                    u.J.push(hui_P2J(p->P.pat(1-i)));
                    QCALL();
                }
            }
            {
                // 2J+2JK=2M,2P+4JK=2M
                for (size_t i = 0; i < 3; i++)
                {
                    zero_struct(u);
                    u.M.push(hui_J2M(p->J.pat((i+1)%3)));
                    u.M.push(hui_J2M(p->J.pat((i+2)%3)));
                    u.M.push(hui_P2M(p->P.pat(0)));
                    u.M.push(hui_P2M(p->P.pat(1)));
                    u.J.push(p->J[i]);
                    QCALL();
                }
            }
        }

        // 7J
        // 2M+1J
        if(CHECK_MDJP(2, 0, 1, 0)) {
            // 2M+6JK=6J
            zero_struct(u);
            for(auto&m:p->M) {
                u.J.push(hui_P2J_1(hui_P(m.idx[0])));
                u.J.push(hui_P2J_1(hui_P(m.idx[1])));
                u.J.push(hui_P2J_1(hui_P(m.idx[2])));
            }
            u.J.push(p->J[0]);
            QCALL();
        }
        // 1M+1D+1J+1P
        if(CHECK_MDJP(1, 1, 1, 1)) {
            // M+3JK=3J,D+2JK=2J,P+JK=J
            zero_struct(u);
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[2])));
            u.J.push(hui_P2J_1(hui_P(p->D.begin()->idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->D.begin()->idx[1])));
            u.J.push(hui_P2J(p->P.begin()));
            u.J.push(p->J[0]);
            QCALL();
        }
        //1M+2J+1P
        if(CHECK_MDJP(1, 0, 2, 1)) {
            // M+3JK=3J,P+JK=J,2JK=J
            zero_struct(u);
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[2])));
            u.J.push(hui_P2J(p->P.begin()));
            u.J.push(hui_J_2JK());
            u.J = p->J;
            QCALL();
        }
        // 1M+1J+3P
        if(CHECK_MDJP(1, 0, 1, 3)) {
            // M+3JK=3J,3P+3JK=3J
            zero_struct(u);
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[0])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[1])));
            u.J.push(hui_P2J_1(hui_P(p->M.begin()->idx[2])));
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_P2J(p->P.pat(2)));
            u.J.push(p->J[0]);
            QCALL();
        }
        // 3D+1J
        if(CHECK_MDJP(0, 3, 1, 0)) {
            // 3D+6JK=6J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            QCALL();
        }
        // 2D+2J
        if(CHECK_MDJP(0, 2, 2, 0)) {
            // 2D+4JK=4J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            QCALL();
        }
        // 2D+1J+2P
        if(CHECK_MDJP(0, 2, 1, 2)) {
            // 2D+4JK=4J,2P+2JK=2J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            QCALL();
        }
        // 1D+2J+2P
        if(CHECK_MDJP(0, 1, 2, 2)) {
            // D+2JK=2J,2P+2JK=2J,2JK=J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 1D+3J
        if(CHECK_MDJP(0, 1, 3, 0)) {
            // D+2JK=2J,4JK=2J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 1D+1J+4P
        if(CHECK_MDJP(0, 1, 1, 4)) {
            // D+2JK=2J,4P+4JK=4J
            zero_struct(u);
            u.J = p->J;
            for(auto&d:p->D) {
                u.J.push(hui_P2J_1(hui_P(d.idx[0])));
                u.J.push(hui_P2J_1(hui_P(d.idx[1])));
            }
            for(auto&p1:p->P) {
                u.J.push(hui_P2J(&p1));
            }
            QCALL();
        }
        // 4J
        if(CHECK_MDJP(0, 0, 4, 0)) {
            // 6JK=3J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 3J+2P
        if(CHECK_MDJP(0, 0, 3, 2)) {
            // 2P+2JK=2J,4JK=2J
            zero_struct(u);
            u.J = p->J;
            u.J.push(hui_P2J(p->P.pat(0)));
            u.J.push(hui_P2J(p->P.pat(1)));
            u.J.push(hui_J_2JK());
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 2J+4P
        if(CHECK_MDJP(0, 0, 2, 4)) {
            // 4P+4JK=4J,2JK=J
            zero_struct(u);
            u.J = p->J;
            for(auto&p1:p->P) {
                u.J.push(hui_P2J(&p1));
            }
            u.J.push(hui_J_2JK());
            QCALL();
        }
        // 1J+6P
        if(CHECK_MDJP(0, 0, 1, 6)) {
            // 6P+6JK=6J
            zero_struct(u);
            u.J = p->J;
            for(auto&p1:p->P) {
                u.J.push(hui_P2J(&p1));
            }
            QCALL();
        }
    }
}

TEST_SUITE_BEGIN("mdjp_all_units");
TEST_CASE("mdjk_hu_0jk") {
    // 4M+1J
    hand_card_units hcu;
    zero_struct(hcu);
    hui_init(hcu.M.grap(), {Wan(1),Wan(2),Wan(3)}, UnitType::UIT_M, UnitSubType::M_BIAN);
    hui_init(hcu.M.grap(), {Wan(3),Wan(4),Wan(5)}, UnitType::UIT_M, UnitSubType::M_ZHONG);
    hui_init(hcu.M.grap(), {Wan(9),Wan(9),Wan(9)}, UnitType::UIT_M, UnitSubType::M_KEZI);
    hui_init(hcu.M.grap(), {Wan(7),Wan(7),Wan(7)}, UnitType::UIT_M, UnitSubType::M_KEZI);
    hui_init(hcu.J.grap(), {Tiao(1), Tiao(1)}, UnitType::UIT_J, UnitSubType::J_19);
    std::stringstream ss_hcu;
    print_hand_card_units(&hcu, ss_hcu);

    mix_hu_mdjp_travel(0, &hcu, &ss_hcu,[](hu_card_units* hui, void* ud){
        std::stringstream* ss_hcu = static_cast<std::stringstream*>(ud);
        std::stringstream ss_hui;
        print_hu_card_units(hui, ss_hui);
        CHECK_EQ(ss_hcu->str(), ss_hui.str());
        return false;
    });

    // 7J
    zero_struct(hcu);
    hui_init(hcu.J.grap(), {Tiao(1), Tiao(1)}, UnitType::UIT_J, UnitSubType::J_19);
    hui_init(hcu.J.grap(), {Wan(1), Wan(1)}, UnitType::UIT_J, UnitSubType::J_19);
    hui_init(hcu.J.grap(), {Dong(1), Dong(1)}, UnitType::UIT_J, UnitSubType::J_19);
    hui_init(hcu.J.grap(), {Tiao(2), Tiao(2)}, UnitType::UIT_J, UnitSubType::J_28);
    hui_init(hcu.J.grap(), {Tiao(3), Tiao(3)}, UnitType::UIT_J, UnitSubType::J_ZHONG);
    hui_init(hcu.J.grap(), {Tiao(4), Tiao(4)}, UnitType::UIT_J, UnitSubType::J_ZHONG);
    hui_init(hcu.J.grap(), {Tiao(5), Tiao(5)}, UnitType::UIT_J, UnitSubType::J_ZHONG);
    ss_hcu.str("");
    print_hand_card_units(&hcu, ss_hcu);
    mix_hu_mdjp_travel(0, &hcu, &ss_hcu,[](hu_card_units* hui, void* ud){
        std::stringstream* ss_hcu = static_cast<std::stringstream*>(ud);
        std::stringstream ss_hui;
        print_hu_card_units(hui, ss_hui);
        CHECK_EQ(ss_hcu->str(), ss_hui.str());
        return false;
    });
}
TEST_CASE("mdjk_hu_1jk") {
    // 4M+1P
    hand_card_units hcu;
    zero_struct(hcu);
    hui_init(hcu.M.grap(), {Wan(1),Wan(2),Wan(3)}, UnitType::UIT_M, UnitSubType::M_BIAN);
    hui_init(hcu.M.grap(), {Wan(3),Wan(4),Wan(5)}, UnitType::UIT_M, UnitSubType::M_ZHONG);
    hui_init(hcu.M.grap(), {Wan(9),Wan(9),Wan(9)}, UnitType::UIT_M, UnitSubType::M_KEZI);
    hui_init(hcu.M.grap(), {Wan(7),Wan(7),Wan(7)}, UnitType::UIT_M, UnitSubType::M_KEZI);
    hui_init(hcu.P.grap(), {Tiao(3)}, UnitType::UIT_P, UnitSubType::P_ZHONG);

    mix_hu_mdjp_travel(1, &hcu, nullptr,[](hu_card_units* hui, void* ud){
        return false;
    });

}
TEST_CASE("mdjk_hu_2jk") {
    
}
TEST_CASE("mdjk_hu_3jk") {
    
}
TEST_CASE("mdjk_hu_4jk") {
    
}
TEST_CASE("mdjk_hu_5jk") {
    
}
TEST_CASE("mdjk_hu_6jk") {
    
}
TEST_SUITE_END();