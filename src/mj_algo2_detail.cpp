#include "mj_algo2_detail.h"
#include <unordered_map>

using namespace mj_algo2;

struct nj_algo2_detail_cache {
    std::unordered_map<cards_key,cardvalues> keys[JOKER_MAX];
};

void mj_algo2::gen_cache()
{

}
