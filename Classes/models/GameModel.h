#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "CardModel.h"
#include <vector>
#include <map>

/**
 * @class GameModel
 * @brief 运行时游戏数据模型
 *
 * 职责说明：
 * - 存储所有卡牌数据
 * - 维护游戏区域和堆叠卡牌集合
 * - 提供卡牌查找和管理辅助函数
 */
class GameModel
{
private:
    std::vector<CardModel*> _playfieldCards;    // 当前在游戏区域的卡牌（主堆叠）
    std::vector<CardModel*> _stackCards;        // 抽牌堆中剩余的卡牌（备用堆叠）
    std::vector<CardModel*> _handStackCards;    // 手牌/托盘堆叠（历史保留，顶部=最后）
    std::map<int, CardModel*> _cardMap;         // ID到卡牌的查找映射
    int _nextCardId;                            // 分配卡牌ID的计数器

public:
    GameModel();
    ~GameModel();

    /**
     * @brief 向游戏区域集合添加卡牌
     * @param card 卡牌实例
     */
    void addPlayfieldCard(CardModel* card);

    /**
     * @brief 向堆叠集合添加卡牌
     * @param card 卡牌实例
     */
    void addStackCard(CardModel* card);

    /**
     * @brief 通过标识符获取卡牌
     * @param cardId 卡牌标识符
     * @return 卡牌指针，如果未找到则返回nullptr
     */
    CardModel* getCardById(int cardId) const;

    /**
     * @brief 获取当前顶部托盘卡牌（手牌堆叠的顶部）
     * @return 托盘顶部卡牌的指针
     */
    CardModel* getTopTrayCard() const;

    /**
     * @brief 将卡牌推入手牌堆叠（成为新的顶部卡牌）
     * @param card 要推送的卡牌实例
     */
    void pushHandStackCard(CardModel* card);

    /**
     * @brief 获取手牌堆叠卡牌列表
     * @return 手牌堆叠向量的引用
     */
    const std::vector<CardModel*>& getHandStackCards() const;

    /**
     * @brief 获取游戏区域卡牌列表
     * @return 游戏区域向量的引用
     */
    const std::vector<CardModel*>& getPlayfieldCards() const;
    std::vector<CardModel*>& getPlayfieldCards();

    /**
     * @brief 获取堆叠卡牌列表
     * @return 堆叠向量的引用
     */
    const std::vector<CardModel*>& getStackCards() const;
    std::vector<CardModel*>& getStackCards();
    
    /**
     * @brief 获取手牌堆叠卡牌列表（非const版本）
     * @return 手牌堆叠向量的引用
     */
    std::vector<CardModel*>& getHandStackCards();

    /**
     * @brief 通过标识符从游戏区域移除卡牌
     * @param cardId 卡牌标识符
     */
    void removePlayfieldCard(int cardId);

    /**
     * @brief 通过标识符从堆叠移除卡牌
     * @param cardId 卡牌标识符
     */
    void removeStackCard(int cardId);

    /**
     * @brief 重新计算哪些游戏区域卡牌应该正面朝上
     */
    void updatePlayfieldVisibility();

    /**
     * @brief 重新计算哪些堆叠卡牌应该正面朝上
     */
    void updateStackVisibility();

    /**
     * @brief 从堆叠集合弹出顶部卡牌
     * @return 卡牌指针，如果堆叠为空则返回nullptr
     */
    CardModel* popStackCard();

    /**
     * @brief 获取下一个可用的卡牌标识符
     * @return 数字卡牌标识符
     */
    int getNextCardId();

    /**
     * @brief 清除所有存储的数据并重置状态
     */
    void clear();
};

#endif // __GAME_MODEL_H__

