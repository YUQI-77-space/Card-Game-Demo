#ifndef __ROUND_STATE_H__
#define __ROUND_STATE_H__

#include "CardStackState.h"

/**
 * @class RoundState
 * @brief 表示单个回合的完整游戏状态
 *
 * 职责说明：
 * - 存储所有三个卡牌堆叠的状态（手牌、备用、主堆叠）
 * - 支持完整的游戏状态快照
 * - 支持状态恢复以实现回滚功能
 */
class RoundState
{
private:
    int _roundNumber;                       // 回合号（基于0）
    CardStackState _handStack;              // 手牌/托盘堆叠（顶部卡牌可见）
    CardStackState _alternativeStack;       // 备用/堆叠卡牌
    CardStackState _mainStack;              // 主堆叠/游戏区域卡牌

public:
    /**
     * @brief 构造函数
     * @param roundNumber 回合号
     */
    RoundState(int roundNumber = 0);

    /**
     * @brief 析构函数
     */
    ~RoundState();

    /**
     * @brief 获取回合号
     * @return 回合号
     */
    int getRoundNumber() const;

    /**
     * @brief 设置回合号
     * @param roundNumber 回合号
     */
    void setRoundNumber(int roundNumber);

    /**
     * @brief 访问手牌堆叠状态
     * @return 手牌堆叠的引用
     */
    CardStackState& getHandStack();
    const CardStackState& getHandStack() const;

    /**
     * @brief 访问备用堆叠状态
     * @return 备用堆叠的引用
     */
    CardStackState& getAlternativeStack();
    const CardStackState& getAlternativeStack() const;

    /**
     * @brief 访问主堆叠状态
     * @return 主堆叠的引用
     */
    CardStackState& getMainStack();
    const CardStackState& getMainStack() const;

    /**
     * @brief 创建此回合状态的深拷贝
     * @return 新的RoundState实例
     */
    RoundState* clone() const;
};

#endif // __ROUND_STATE_H__

