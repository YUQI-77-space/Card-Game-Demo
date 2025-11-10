#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

/**
 * @class CardView
 * @brief 卡牌的可视化表示
 *
 * 职责说明：
 * - 使用背景、花色图标和数字精灵渲染卡牌
 * - 处理卡牌的触摸交互
 * - 提供辅助动画，如移动和翻转
 */
class CardView : public cocos2d::Node
{
private:
    cocos2d::Sprite* _backgroundSprite;         // 卡牌背景精灵
    cocos2d::Sprite* _backSprite;               // 卡牌背面精灵
    cocos2d::Sprite* _suitSprite;               // 花色图标精灵
    cocos2d::Sprite* _bigNumberSprite;          // 大数字精灵（中心）
    cocos2d::Sprite* _smallNumberSprite;        // 小数字精灵（角落）
    const CardModel* _cardModel;                // 关联的卡牌模型（只读）
    std::function<void(int)> _clickCallback;    // 点击回调函数
    cocos2d::EventListenerTouchOneByOne* _touchListener;  // 触摸事件监听器

public:
    /**
     * @brief 工厂方法创建CardView
     * @param model 要绑定的卡牌模型
     * @return CardView实例
     */
    static CardView* create(const CardModel* model);

    /**
     * @brief 初始化卡牌视图
     * @param model 要绑定的卡牌模型
     * @return 成功返回true，否则返回false
     */
    bool init(const CardModel* model);

    /**
     * @brief 析构函数
     */
    virtual ~CardView();

    /**
     * @brief 设置卡牌点击回调函数
     * @param callback 使用卡牌ID调用的回调函数
     */
    void setClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 播放移动到目标位置的动画
     * @param targetPos 目标位置
     * @param callback 可选的完成回调函数
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPos, const std::function<void()>& callback = nullptr);

    /**
     * @brief 播放简单的翻转动画
     * @param callback 可选的完成回调函数
     */
    void playFlipAnimation(const std::function<void()>& callback = nullptr);

    /**
     * @brief 根据模型更新可见状态
     */
    void updateDisplay();

    /**
     * @brief 获取关联的卡牌模型
     * @return 卡牌模型的指针
     */
    const CardModel* getCardModel() const;

private:
    /**
     * @brief 初始化触摸处理
     */
    void _initTouchEvent();

    /**
     * @brief 触摸开始回调函数
     * @param touch 触摸信息
     * @param event 事件信息
     * @return 当卡牌应该处理触摸时返回true
     */
    bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __CARD_VIEW_H__

