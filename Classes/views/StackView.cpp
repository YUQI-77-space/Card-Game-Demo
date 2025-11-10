#include "StackView.h"

USING_NS_CC;

StackView* StackView::create()
{
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool StackView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _topTrayCardView = nullptr;
    
    return true;
}

void StackView::addStackCardView(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    _stackCardViews.push_back(cardView);
    
    // 使用索引作为z-order，使后面的卡牌堆叠在顶部
    int zOrder = static_cast<int>(_stackCardViews.size());
    this->addChild(cardView, zOrder);
    
    // 注册点击回调函数
    cardView->setClickCallback([this](int cardId) {
        if (_onCardClickCallback)
        {
            _onCardClickCallback(cardId);
        }
    });

    cardView->updateDisplay();
    
    // 更新堆叠卡牌位置
    _updateStackCardsPosition();
}

void StackView::setTopTrayCardView(CardView* cardView)
{
    if (_topTrayCardView == cardView)
    {
        if (_topTrayCardView)
        {
            _topTrayCardView->updateDisplay();
        }
        return;
    }

    CardView* previousTop = _topTrayCardView;
    _topTrayCardView = cardView;
    
    if (_topTrayCardView)
    {
        // 避免重新添加同一节点，这会触发cocos2d-x的断言
        if (_topTrayCardView->getParent() != this)
        {
            if (_topTrayCardView->getParent() != nullptr)
            {
                _topTrayCardView->retain();
                _topTrayCardView->removeFromParent();
                // 托盘卡牌应该在所有内容之上（使用高z-order）
                this->addChild(_topTrayCardView, 10000);
                _topTrayCardView->release();
            }
            else
            {
                // 托盘卡牌应该在所有内容之上（使用高z-order）
                this->addChild(_topTrayCardView, 10000);
            }
        }
        else
        {
            // 如果已经是子节点，更新其z-order以确保它在顶部
            _topTrayCardView->setLocalZOrder(10000);
        }
        
        // 注册点击回调函数
        _topTrayCardView->setClickCallback([this](int cardId) {
            if (_onCardClickCallback)
            {
                _onCardClickCallback(cardId);
            }
        });

        _topTrayCardView->updateDisplay();
    }

    if (previousTop && previousTop != _topTrayCardView)
    {
        previousTop->removeFromParent();
    }
}

CardView* StackView::getTopTrayCardView() const
{
    return _topTrayCardView;
}

void StackView::setOnCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
}

CardView* StackView::getCardViewById(int cardId) const
{
    // 在堆叠卡牌列表中搜索
    for (auto cardView : _stackCardViews)
    {
        if (cardView->getCardModel() && cardView->getCardModel()->getCardId() == cardId)
        {
            return cardView;
        }
    }
    
    // 最后检查托盘卡牌
    if (_topTrayCardView && _topTrayCardView->getCardModel() && 
        _topTrayCardView->getCardModel()->getCardId() == cardId)
    {
        return _topTrayCardView;
    }
    
    return nullptr;
}

void StackView::removeStackCardReference(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }

    for (auto it = _stackCardViews.begin(); it != _stackCardViews.end(); ++it)
    {
        if (*it == cardView)
        {
            _stackCardViews.erase(it);
            break;
        }
    }

    _updateStackCardsPosition();
}

void StackView::refreshStackCardDisplays()
{
    // 刷新所有堆叠卡牌视图的显示
    for (CardView* cardView : _stackCardViews)
    {
        if (cardView)
        {
            cardView->updateDisplay();
        }
    }

    // 刷新托盘卡牌视图的显示
    if (_topTrayCardView)
    {
        _topTrayCardView->updateDisplay();
    }

    // 显示更新后重新应用堆叠位置，使卡牌保持扇形排列
    _updateStackCardsPosition();
}

void StackView::clearAllStackCards()
{
    // 从场景中移除所有堆叠卡牌视图
    for (CardView* cardView : _stackCardViews)
    {
        if (cardView)
        {
            cardView->removeFromParent();
        }
    }
    _stackCardViews.clear();
}

void StackView::_updateStackCardsPosition()
{
    // 使用水平偏移排列堆叠卡牌以显示多张卡牌
    static const float kHorizontalOffset = 50.0f;  // 卡牌之间的水平间距
    static const float kBaseX = 200.0f;             // 基础X位置
    static const float kBaseY = 290.0f;             // 基础Y位置
    
    // 为每张卡牌设置位置，水平偏移使其可见
    for (size_t i = 0; i < _stackCardViews.size(); i++)
    {
        CardView* cardView = _stackCardViews[i];
        // 每张卡牌水平偏移使其可见
        float offsetX = i * kHorizontalOffset;
        cardView->setPosition(Vec2(kBaseX + offsetX, kBaseY));
        
        CCLOG("StackView: 卡牌 [%d] 定位在 (%.0f, %.0f)", 
              (int)i, kBaseX + offsetX, kBaseY);
    }
}

