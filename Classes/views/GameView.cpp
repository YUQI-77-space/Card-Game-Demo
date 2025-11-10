#include "GameView.h"

USING_NS_CC;

GameView* GameView::create()
{
    GameView* ret = new (std::nothrow) GameView();
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

bool GameView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // 初始化容器
    _initContainers();
    
    return true;
}

cocos2d::Node* GameView::getPlayfieldContainer() const
{
    return _playfieldContainer;
}

cocos2d::Node* GameView::getStackContainer() const
{
    return _stackContainer;
}

void GameView::addCardView(CardView* cardView, bool isPlayfield)
{
    if (!cardView)
    {
        return;
    }
    
    _cardViews.push_back(cardView);
    
    if (isPlayfield)
    {
        _playfieldContainer->addChild(cardView);
    }
    else
    {
        _stackContainer->addChild(cardView);
    }
}

void GameView::removeCardView(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    // 从跟踪所有视图的列表中移除
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

CardView* GameView::getCardViewById(int cardId) const
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

void GameView::_initContainers()
{
    // 创建游戏区域容器（1080 x 1500）
    _playfieldContainer = Node::create();
    _playfieldContainer->setContentSize(Size(1080, 1500));
    _playfieldContainer->setPosition(0, 580);  // 位于堆叠区域上方
    this->addChild(_playfieldContainer);
    
    // 为游戏区域添加绿色背景
    auto playfieldBg = LayerColor::create(Color4B(34, 139, 34, 255), 1080, 1500);
    _playfieldContainer->addChild(playfieldBg, -1);
    
    // 创建堆叠容器（1080 x 580）
    _stackContainer = Node::create();
    _stackContainer->setContentSize(Size(1080, 580));
    _stackContainer->setPosition(0, 0);  // 位于底部
    this->addChild(_stackContainer);
    
    // 为堆叠区域添加棕色背景
    auto stackBg = LayerColor::create(Color4B(101, 67, 33, 255), 1080, 580);
    _stackContainer->addChild(stackBg, -1);
}

