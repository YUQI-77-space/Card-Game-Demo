#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <vector>

/**
 * @class GameView
 * @brief 负责视觉布局的根层
 *
 * 职责说明：
 * - 管理整体游戏UI布局
 * - 提供游戏区域和堆叠区域的容器
 * - 拥有并管理所有CardView实例
 */
class GameView : public cocos2d::Layer
{
private:
    cocos2d::Node* _playfieldContainer;     // 游戏区域容器（1080 x 1500）
    cocos2d::Node* _stackContainer;         // 堆叠容器（1080 x 580）
    std::vector<CardView*> _cardViews;      // 所有活动的卡牌视图

public:
    /**
     * @brief 工厂方法创建GameView
     * @return 新创建的GameView实例
     */
    static GameView* create();

    /**
     * @brief 初始化视图
     * @return 成功返回true，否则返回false
     */
    virtual bool init();

    /**
     * @brief 获取游戏区域容器节点
     * @return 表示游戏区域的节点
     */
    cocos2d::Node* getPlayfieldContainer() const;

    /**
     * @brief 获取堆叠容器节点
     * @return 表示堆叠区域的节点
     */
    cocos2d::Node* getStackContainer() const;

    /**
     * @brief 向适当区域添加卡牌视图
     * @param cardView 卡牌视图实例
     * @param isPlayfield 当卡牌属于游戏区域时为true
     */
    void addCardView(CardView* cardView, bool isPlayfield);

    /**
     * @brief 从场景图中移除卡牌视图
     * @param cardView 卡牌视图实例
     */
    void removeCardView(CardView* cardView);

    /**
     * @brief 通过卡牌标识符查找卡牌视图
     * @param cardId 卡牌标识符
     * @return 卡牌视图指针或nullptr
     */
    CardView* getCardViewById(int cardId) const;

private:
    /**
     * @brief 初始化游戏区域和堆叠容器
     */
    void _initContainers();
};

#endif // __GAME_VIEW_H__

