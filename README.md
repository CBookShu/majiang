### 主题
    探讨一下麻将的各种计算：
    1. 计算组合能不能胡
    2. 将手牌可以胡的各种组合遍历出来
    3. 探讨性能的优化
### 基础概念
    牌ID: 0~135
    即:
    万: 1~9  每个4张
    筒: 1~9  每个4张
    条: 1~9  每个4张
    风板: 东南西北中发白 每个4张
    共135张

    麻将的一些名词：
    万: W, 条: T 筒: D, 风板:F, 财: JOKER
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
    1. 快速计算是否能胡
    2. 遍历所有胡牌的组合
    3. 机器人AI的思考

### 算法
[回溯算法](BacktraceAlgo.md)

[缓存查表](CacheAlgo.md)

[优化回溯算法](TotalBackTraceAlgo.md)