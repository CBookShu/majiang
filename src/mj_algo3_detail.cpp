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

#define MAKE_MDJP_TABLE(m,d,j,p,jk) ChecAndGet<m,d,j,p,jk>()
template<int M,int D,int J,int P, int JK>
static hu_units_des ChecAndGet() {
    static_assert((M*3 + 2*D + 2*J + P + JK) == HAND_CARDS_COUNT);
    return hu_units_des{M,D,J,P};
}


int get_hu_mdjp_des_4m1j(int joker, hu_units_des*& des)
{
    if(joker == 0) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(4,0,1,0,0)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if (joker == 1) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(4,0,0,1,1),MAKE_MDJP_TABLE(3,1,1,0,1),MAKE_MDJP_TABLE(3,0,2,0,1)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if(joker == 2) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(4,0,0,0,2),MAKE_MDJP_TABLE(3,0,1,1,2),
            MAKE_MDJP_TABLE(3,1,0,1,2),MAKE_MDJP_TABLE(2,2,1,0,2),
            MAKE_MDJP_TABLE(2,1,2,0,2),MAKE_MDJP_TABLE(2,0,3,0,2),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 3) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(3,0,1,0,3),MAKE_MDJP_TABLE(3,1,0,0,3),
            MAKE_MDJP_TABLE(3,0,0,2,3),MAKE_MDJP_TABLE(2,2,0,1,3),
            MAKE_MDJP_TABLE(2,1,1,1,3),MAKE_MDJP_TABLE(2,0,2,1,3),
            MAKE_MDJP_TABLE(1,0,4,0,3),MAKE_MDJP_TABLE(1,1,3,0,3),
            MAKE_MDJP_TABLE(1,2,2,0,3),MAKE_MDJP_TABLE(1,3,1,0,3),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 4) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(3,0,0,1,4),MAKE_MDJP_TABLE(2,2,0,0,4),
            MAKE_MDJP_TABLE(2,1,1,0,4),MAKE_MDJP_TABLE(2,1,0,2,4),
            MAKE_MDJP_TABLE(2,0,2,0,4),MAKE_MDJP_TABLE(2,0,1,2,4),
            MAKE_MDJP_TABLE(1,3,0,1,4),MAKE_MDJP_TABLE(1,2,1,1,4),
            MAKE_MDJP_TABLE(1,1,2,1,4),MAKE_MDJP_TABLE(1,0,3,1,4),
            MAKE_MDJP_TABLE(0,4,1,0,4),MAKE_MDJP_TABLE(0,3,2,0,4),
            MAKE_MDJP_TABLE(0,2,3,0,4),MAKE_MDJP_TABLE(0,1,4,0,4),
            MAKE_MDJP_TABLE(0,0,5,0,4),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 5) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(3,0,0,0,5),MAKE_MDJP_TABLE(2,1,0,1,5),
            MAKE_MDJP_TABLE(2,0,1,1,5),MAKE_MDJP_TABLE(2,0,0,3,5),
            MAKE_MDJP_TABLE(1,3,0,0,5),MAKE_MDJP_TABLE(1,2,1,0,5),
            MAKE_MDJP_TABLE(1,2,0,2,5),MAKE_MDJP_TABLE(1,1,2,0,5),
            MAKE_MDJP_TABLE(1,1,1,2,5),MAKE_MDJP_TABLE(1,0,3,0,5),
            MAKE_MDJP_TABLE(1,0,2,2,5),MAKE_MDJP_TABLE(0,0,4,1,5),
            MAKE_MDJP_TABLE(0,4,0,1,5),MAKE_MDJP_TABLE(0,3,1,1,5),
            MAKE_MDJP_TABLE(0,2,2,1,5),MAKE_MDJP_TABLE(0,1,3,1,5),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 6) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(2,1,0,0,6),MAKE_MDJP_TABLE(2,0,1,0,6),
            MAKE_MDJP_TABLE(2, 0, 0, 2, 6),MAKE_MDJP_TABLE(1, 2, 0, 1, 6),
            MAKE_MDJP_TABLE(1, 1, 1, 1, 6),MAKE_MDJP_TABLE(1, 1, 0, 3, 6),
            MAKE_MDJP_TABLE(1, 0, 2, 1, 6),MAKE_MDJP_TABLE(1, 0, 1, 3, 6),
            MAKE_MDJP_TABLE(0, 4, 0, 0, 6),MAKE_MDJP_TABLE(0, 3, 1, 0, 6),
            MAKE_MDJP_TABLE(0, 3, 0, 2, 6),MAKE_MDJP_TABLE(0, 2, 2, 0, 6),
            MAKE_MDJP_TABLE(0, 2, 1, 2, 6),MAKE_MDJP_TABLE(0, 1, 3, 0, 6),
            MAKE_MDJP_TABLE(0, 1, 2, 2, 6),MAKE_MDJP_TABLE(0, 0, 4, 0, 6),
            MAKE_MDJP_TABLE(0, 0, 3, 2, 6),
        };
        des = &g_des[0];
        return std::size(g_des);
    }

    assert(false);
    return -1;
}

int get_hu_mdjp_des_7j(int joker, hu_units_des *&des)
{
    if(joker == 0) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(0,0,7,0,0)
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if (joker == 1) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(0,0,6,1,1),
        };
        des = &g_des[0];
        return std::size(g_des);
    } else if(joker == 2) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(0,0,6,0,2),MAKE_MDJP_TABLE(0,0,5,2,2),
            MAKE_MDJP_TABLE(0,1,5,0,2),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 3) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(0,0,5,1,3),MAKE_MDJP_TABLE(0,1,4,1,3),
            MAKE_MDJP_TABLE(0,0,4,3,3),MAKE_MDJP_TABLE(1,0,4,0,3),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 4) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(1,0,3,1,4),MAKE_MDJP_TABLE(0,2,3,0,4),
            MAKE_MDJP_TABLE(0,1,4,0,4),MAKE_MDJP_TABLE(0,0,5,0,4),
            MAKE_MDJP_TABLE(0,0,4,2,4),MAKE_MDJP_TABLE(0,0,3,4,4),
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 5) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(1,1,2,0,5),MAKE_MDJP_TABLE(1,0,3,0,5),
            MAKE_MDJP_TABLE(1,0,2,2,5),MAKE_MDJP_TABLE(0,2,2,1,5),
            MAKE_MDJP_TABLE(0,1,3,1,5),MAKE_MDJP_TABLE(0,1,2,3,5),
            MAKE_MDJP_TABLE(0,0,4,1,5),MAKE_MDJP_TABLE(0,0,3,3,5),
            MAKE_MDJP_TABLE(0,0,2,5,5)
        };
        des = &g_des[0];
        return std::size(g_des);
    }else if(joker == 6) {
        static hu_units_des g_des[] = {
            MAKE_MDJP_TABLE(2, 0, 1, 0, 6),MAKE_MDJP_TABLE(1, 1, 1, 1, 6),
            MAKE_MDJP_TABLE(1, 0, 2, 1, 6),MAKE_MDJP_TABLE(1, 0, 1, 3, 6),
            MAKE_MDJP_TABLE(0, 3, 1, 0, 6),MAKE_MDJP_TABLE(0, 2, 2, 0, 6),
            MAKE_MDJP_TABLE(0, 2, 1, 2, 6),MAKE_MDJP_TABLE(0, 1, 2, 2, 6),
            MAKE_MDJP_TABLE(0, 1, 3, 0, 6),MAKE_MDJP_TABLE(0, 1, 1, 4, 6),
            MAKE_MDJP_TABLE(0, 0, 4, 0, 6),MAKE_MDJP_TABLE(0, 0, 3, 2, 6),
            MAKE_MDJP_TABLE(0, 0, 2, 4, 6),MAKE_MDJP_TABLE(0, 0, 1, 6, 6),
        };
        des = &g_des[0];
        return std::size(g_des);
    }

    assert(false);
    return -1;
}
