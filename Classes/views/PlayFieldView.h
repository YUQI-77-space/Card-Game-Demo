#ifndef __PLAYFIELD_VIEW_H__
#define __PLAYFIELD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <vector>

/**
 * @class PlayFieldView
 * @brief 游戏区域的视图组件
 *
 * 职责说明：
 * - 渲染游戏区域上的所有卡牌
 * - 中继游戏区域卡牌的点击事件
 */
class PlayFieldView : public cocos2d::Node
{
private:
    std::vector<CardView*> _cardViews;              // 游戏区域上的卡牌视图列表
    std::function<void(int)> _onCardClickCallback;  // 卡牌点击回调函数

public:
    /**
     * @brief 工厂方法创建PlayFieldView
     * @return PlayFieldView实例
     */
    static PlayFieldView* create();

    /**
     * @brief 初始化游戏区域视图
     * @return 成功返回true，否则返回false
     */
    virtual bool init();

    /**
     * @brief 向游戏区域添加卡牌视图
     * @param cardView 卡牌视图实例
     */
    void addCardView(CardView* cardView);

    /**
     * @brief 从游戏区域移除卡牌视图
     * @param cardView 卡牌视图实例
     */
    void removeCardView(CardView* cardView);

    /**
     * @brief 设置游戏区域卡牌被点击时调用的回调函数
     * @param callback 回调函数
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 通过卡牌标识符查找游戏区域卡牌视图
     * @param cardId 卡牌标识符
     * @return 卡牌视图指针，如果未找到则返回nullptr
     */
    CardView* getCardViewById(int cardId) const;

    /**
     * @brief 刷新所有游戏区域卡牌的可视状态
     */
    void refreshCardDisplays();

    /**
     * @brief 清除游戏区域的所有卡牌视图
     */
    void clearAllCards();
};

#endif // __PLAYFIELD_VIEW_H__

