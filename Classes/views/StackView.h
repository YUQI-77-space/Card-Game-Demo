#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <vector>

/**
 * @class StackView
 * @brief 堆叠和托盘区域的视图组件
 *
 * 职责说明：
 * - 使用堆叠视觉效果渲染抽牌堆
 * - 渲染当前托盘卡牌
 * - 中继堆叠卡牌点击事件
 */
class StackView : public cocos2d::Node
{
private:
    std::vector<CardView*> _stackCardViews;         // 表示抽牌堆的卡牌视图
    CardView* _topTrayCardView;                     // 表示托盘顶部卡牌的视图
    std::function<void(int)> _onCardClickCallback;  // 卡牌点击回调函数

public:
    /**
     * @brief 工厂方法创建StackView
     * @return StackView实例
     */
    static StackView* create();

    /**
     * @brief 初始化内部状态
     * @return 成功返回true，否则返回false
     */
    virtual bool init();

    /**
     * @brief 向堆叠集合添加卡牌视图
     * @param cardView 卡牌视图实例
     */
    void addStackCardView(CardView* cardView);

    /**
     * @brief 分配当前托盘卡牌视图
     * @param cardView 卡牌视图实例
     */
    void setTopTrayCardView(CardView* cardView);

    /**
     * @brief 获取托盘卡牌视图
     * @return 托盘卡牌视图的指针
     */
    CardView* getTopTrayCardView() const;

    /**
     * @brief 设置卡牌被点击时调用的回调函数
     * @param callback 回调函数
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 通过卡牌标识符在堆叠中查找卡牌视图
     * @param cardId 卡牌标识符
     * @return 卡牌视图指针或nullptr
     */
    CardView* getCardViewById(int cardId) const;

    /**
     * @brief 从堆叠集合中移除卡牌视图，但不删除节点
     * @param cardView 卡牌视图实例
     */
    void removeStackCardReference(CardView* cardView);

    /**
     * @brief 刷新堆叠卡牌的正面/背面状态
     */
    void refreshStackCardDisplays();

    /**
     * @brief 清除所有堆叠卡牌视图
     */
    void clearAllStackCards();

private:
    /**
     * @brief 更新堆叠卡牌的可视位置
     */
    void _updateStackCardsPosition();
};

#endif // __STACK_VIEW_H__

