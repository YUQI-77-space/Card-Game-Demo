#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "utils/GameTypes.h"
#include <vector>

/**
 * @class CardConfig
 * @brief 单张卡牌条目的配置数据
 */
struct CardConfig
{
    CardFaceType cardFace;      // 卡牌面值
    CardSuitType cardSuit;      // 卡牌花色
    cocos2d::Vec2 position;     // 卡牌位置

    CardConfig()
        : cardFace(CFT_NONE)
        , cardSuit(CST_NONE)
        , position(cocos2d::Vec2::ZERO)
    {}
};

/**
 * @class LevelConfig
 * @brief 关卡配置容器
 *
 * 职责说明：
 * - 存储关卡的静态配置数据
 * - 提供游戏区域和堆叠区域的卡牌列表
 */
class LevelConfig
{
private:
    std::vector<CardConfig> _playfieldCards;    // 游戏区域卡牌配置
    std::vector<CardConfig> _stackCards;        // 堆叠卡牌配置
    int _levelId;                               // 关卡标识符

public:
    LevelConfig();
    ~LevelConfig();

    /**
     * @brief 获取游戏区域卡牌配置
     * @return 游戏区域卡牌配置的向量
     */
    const std::vector<CardConfig>& getPlayfieldCards() const;

    /**
     * @brief 获取堆叠卡牌配置
     * @return 堆叠卡牌配置的向量
     */
    const std::vector<CardConfig>& getStackCards() const;

    /**
     * @brief 获取关卡标识符
     * @return 关卡标识符
     */
    int getLevelId() const;

    /**
     * @brief 设置游戏区域卡牌配置
     * @param cards 游戏区域卡牌配置的向量
     */
    void setPlayfieldCards(const std::vector<CardConfig>& cards);

    /**
     * @brief 设置堆叠卡牌配置
     * @param cards 堆叠卡牌配置的向量
     */
    void setStackCards(const std::vector<CardConfig>& cards);

    /**
     * @brief 设置关卡标识符
     * @param levelId 关卡标识符
     */
    void setLevelId(int levelId);
};

#endif // __LEVEL_CONFIG_H__

