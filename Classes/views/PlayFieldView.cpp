#include "PlayFieldView.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create()
{
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
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

bool PlayFieldView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}

void PlayFieldView::addCardView(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    _cardViews.push_back(cardView);
    
    // 使用Y坐标作为z-order，使位置更高的卡牌渲染在顶部
    // 从大值中减去，使屏幕顶部（Y值更高）的卡牌具有更高的z-order
    int zOrder = static_cast<int>(cardView->getCardModel()->getPosition().y);
    this->addChild(cardView, zOrder);
    
    CCLOG("PlayFieldView::addCardView - 卡牌 %d 在 Y=%.0f, z-order=%d",
          cardView->getCardModel()->getCardId(),
          cardView->getCardModel()->getPosition().y,
          zOrder);
    
    // 注册点击回调函数
    cardView->setClickCallback([this](int cardId) {
        if (_onCardClickCallback)
        {
            _onCardClickCallback(cardId);
        }
    });

    cardView->updateDisplay();
}

void PlayFieldView::removeCardView(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    // 从本地列表中移除
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it)
    {
        if (*it == cardView)
        {
            _cardViews.erase(it);
            break;
        }
    }
    
    // 从父节点移除
    cardView->removeFromParent();
}

void PlayFieldView::setOnCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
}

CardView* PlayFieldView::getCardViewById(int cardId) const
{
    for (auto cardView : _cardViews)
    {
        if (cardView->getCardModel() && cardView->getCardModel()->getCardId() == cardId)
        {
            return cardView;
        }
    }
    return nullptr;
}

void PlayFieldView::refreshCardDisplays()
{
    for (CardView* cardView : _cardViews)
    {
        if (cardView)
        {
            cardView->updateDisplay();
        }
    }
}

void PlayFieldView::clearAllCards()
{
    // 从场景中移除所有卡牌视图
    for (CardView* cardView : _cardViews)
    {
        if (cardView)
        {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
}

