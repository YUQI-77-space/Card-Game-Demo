#ifndef __CARD_STACK_STATE_H__
#define __CARD_STACK_STATE_H__

#include "cocos2d.h"
#include <vector>

/**
 * @struct CardInStack
 * @brief 表示堆叠中单张卡牌的状态
 */
struct CardInStack
{
    int cardId;                 // 唯一卡牌标识符
    cocos2d::Vec2 position;     // 卡牌在屏幕上的位置
    
    CardInStack(int id, const cocos2d::Vec2& pos)
        : cardId(id), position(pos)
    {
    }
};

/**
 * @class CardStackState
 * @brief 表示特定回合时卡牌堆叠的完整状态
 *
 * 职责说明：
 * - 存储堆叠中卡牌的有序列表（顶部=向量的末尾）
 * - 存储每张卡牌的位置信息
 * - 支持状态快照和恢复
 */
class CardStackState
{
private:
    std::vector<CardInStack> _cards;    // 有序卡牌列表（索引0=底部，末尾=顶部）

public:
    /**
     * @brief 构造函数
     */
    CardStackState();

    /**
     * @brief 析构函数
     */
    ~CardStackState();

    /**
     * @brief 向堆叠顶部添加卡牌
     * @param cardId 卡牌标识符
     * @param position 卡牌位置
     */
    void pushCard(int cardId, const cocos2d::Vec2& position);

    /**
     * @brief 从堆叠移除并返回顶部卡牌
     * @param outCardId 输出参数，卡牌ID
     * @param outPosition 输出参数，卡牌位置
     * @return 如果弹出卡牌返回true，如果堆叠为空返回false
     */
    bool popCard(int& outCardId, cocos2d::Vec2& outPosition);

    /**
     * @brief 获取顶部卡牌但不移除它
     * @param outCardId 输出参数，卡牌ID
     * @param outPosition 输出参数，卡牌位置
     * @return 如果堆叠不为空返回true，否则返回false
     */
    bool peekTop(int& outCardId, cocos2d::Vec2& outPosition) const;

    /**
     * @brief 获取堆叠中的卡牌数量
     * @return 卡牌数量
     */
    size_t getCardCount() const;

    /**
     * @brief 检查堆叠是否为空
     * @return 如果为空返回true
     */
    bool isEmpty() const;

    /**
     * @brief 获取堆叠中的所有卡牌
     * @return 卡牌向量的引用
     */
    const std::vector<CardInStack>& getCards() const;

    /**
     * @brief 清除堆叠中的所有卡牌
     */
    void clear();

    /**
     * @brief 设置整个堆叠状态（用于恢复）
     * @param cards 要设置的卡牌列表
     */
    void setCards(const std::vector<CardInStack>& cards);
};

#endif // __CARD_STACK_STATE_H__

