#include "mj_algo3_detail.h"
#include "mj_utils.h"

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
    res.count = hui_D2M(d,res.begin());
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
static int ChecAndGet() {
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

void mix_hu_mdjp_travel(int joker, hand_card_units* p, bool(*func)(hu_card_units* u))
{
    int M = p->M.count;
    int D = p->D.count;
    int J = p->J.count;
    int P = p->P.count;
    int mdjp = calc_mdjp(p);
    hu_card_units u;
#define CHECK_MDJP(m,d,j,p) MAKE_MDJP_TABLE(m,d,j,p)==mdjp
#define QCALL()    if(func(&u)) {return;}
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
        // TODO:
    } else if(joker == 5) {
        // TODO:
    } else if(joker == 6) {
        // TODO:
    }
}
