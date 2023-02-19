### 主题
    探讨：
    108张麻将，带财神，不计风、字等牌
    每个人手牌14张胡
    其基本原理已经覆盖了麻将的一般算法

### 基础概念
    牌ID: 0~107
    即:
    万: 1~9  每个4张
    筒: 1~9  每个4张
    条: 1~9  每个4张
    共108张

    麻将的一些名词：
    万: W, 筒: D, 条: T
    吃，碰，刚: cpg

    顺: Shun    同花色3张连:1W2W3W
    刻: Ke      同花色3张相同:1W2W3W
    将: Jiang   同花色2张相同:1W1W
    单: Dan     一张牌:1W
    连: Lian    同花色2张连:1W2W
    坎: Kan     同花色2张差1相连:1W3W,5W7W

    此外还有一些更细致的组合属性:
    单: 1,2,8,9 为边张，即它们左右可以相连的比较少
    连: 12,89 为边张 

    牌张组合:
    M = {Shun | Ke},
    D = {Lian | Kan}
    J = Jiang
    P = Dan

    Hu: 
        1.  (M + cpg) + 1J
        2.  7J
    
### 计算内容
    1. 快速计算胡牌
    2. 计算听牌
    3. 遍历基本组合【可以根据业务计算最大胡翻】

### 算法
    v1. 动态规划
    v2. 缓存查表
    v3. 优化动态规划，超越查表