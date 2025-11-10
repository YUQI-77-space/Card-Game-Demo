#include "StackController.h"

USING_NS_CC;

StackController::StackController(GameModel* gameModel, GameMemory* gameMemory)
    : _gameModel(gameModel)
    , _stackView(nullptr)
    , _gameMemory(gameMemory)
{
}

StackController::~StackController()
{
    // _stackView由Cocos2d-x管理，无需手动删除
}

bool StackController::init()
{
    if (!_gameModel)
    {
        return false;
    }
    
    return true;
}

void StackController::initView(cocos2d::Node* parent)
{
    if (!parent || !_gameModel)
    {
        return;
    }
    
    // 创建堆叠视图
    _stackView = StackView::create();
    parent->addChild(_stackView);
    
    // 注册卡牌点击回调函数
    _stackView->setOnCardClickCallback([this](int cardId) {
        this->handleStackCardClick(cardId);
    });
    
    // 创建堆叠卡牌的视觉显示
    const std::vector<CardModel*>& stackCards = _gameModel->getStackCards();
    for (CardModel* card : stackCards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            _stackView->addStackCardView(cardView);
        }
    }
    
    // 创建托盘卡牌视图
    CardModel* topTrayCard = _gameModel->getTopTrayCard();
    if (topTrayCard)
    {
        CardView* cardView = CardView::create(topTrayCard);
        if (cardView)
        {
            _stackView->setTopTrayCardView(cardView);
        }
    }

    // 刷新堆叠卡牌显示
    if (_stackView)
    {
        _stackView->refreshStackCardDisplays();
    }
}

void StackController::handleStackCardClick(int cardId)
{
    CCLOG("堆叠卡牌被点击: %d", cardId);
    
    // 验证被点击的卡牌是否来自堆叠区域
    const std::vector<CardModel*>& stackCards = _gameModel->getStackCards();
    bool isStackCard = false;
    
    for (CardModel* card : stackCards)
    {
        if (card->getCardId() == cardId)
        {
            isStackCard = true;
            break;
        }
    }
    
    if (isStackCard)
    {
        // 点击的是堆叠卡牌，执行替换操作
        replaceTopTrayCard(cardId);
    }
}

void StackController::replaceTopTrayCard(int cardId)
{
    if (!_gameModel || !_stackView)
    {
        return;
    }
    
    // 步骤1：获取被点击的卡牌模型
    CardModel* clickedCard = _gameModel->getCardById(cardId);
    if (!clickedCard)
    {
        return;
    }
    
    // 步骤2：获取当前托盘卡牌模型
    CardModel* currentTopCard = _gameModel->getTopTrayCard();
    if (!currentTopCard)
    {
        return;
    }
    
    // 步骤3：获取对应的视图对象
    CardView* clickedCardView = _stackView->getCardViewById(cardId);
    CardView* topTrayCardView = _stackView->getTopTrayCardView();
    
    if (!clickedCardView || !topTrayCardView)
    {
        return;
    }

    // 步骤4：在状态改变之前缓存起始位置和目标位置
    // 使用实际视图位置以尊重当前的水平偏移布局
    cocos2d::Vec2 fromPos = clickedCardView->getPosition();
    cocos2d::Vec2 targetPos = currentTopCard->getPosition();
    
    CCLOG("=== 堆叠卡牌替换动画开始 ===");
    CCLOG("移动卡牌 %d 从 (%.0f, %.0f) 到 (%.0f, %.0f)", 
          clickedCard->getCardId(), fromPos.x, fromPos.y, targetPos.x, targetPos.y);
    
    // 步骤5：确保卡牌显示正面
    clickedCard->setFlipped(true);
    clickedCardView->updateDisplay();
    
    // 步骤6：动画期间保持卡牌视图可见
    // 从堆叠引用中移除，但保持可见状态
    _stackView->removeStackCardReference(clickedCardView);
    
    // 步骤7：确保卡牌从正确位置开始
    clickedCardView->setPosition(fromPos);
    
    // 步骤8：将卡牌提升到顶层以便流畅动画
    clickedCardView->setLocalZOrder(10000);
    
    // 步骤9：播放移动动画 - 完成从A到B的移动
    clickedCardView->playMoveAnimation(targetPos, [this, clickedCard, targetPos, clickedCardView]() {
        CCLOG("动画完成，更新模型状态...");
        
        // 步骤10：动画完成后更新模型状态
        clickedCard->setPosition(targetPos);
        
        // 步骤11：从备用堆叠中移除卡牌
        _gameModel->removeStackCard(clickedCard->getCardId());
        
        // 步骤12：将卡牌推入手牌堆叠（成为新的顶部卡牌）
        _gameModel->pushHandStackCard(clickedCard);
        
        CCLOG("已将卡牌 %d 推入手牌堆叠", clickedCard->getCardId());

        // 步骤13：模型更改后更新视图
        if (_stackView)
        {
            _stackView->setTopTrayCardView(clickedCardView);
            _stackView->refreshStackCardDisplays();
        }
        
        // 步骤14：所有操作完成后保存新的回合状态
        if (_gameMemory)
        {
            _gameMemory->saveCurrentRound();
        }
        
        CCLOG("=== 堆叠替换完成 ===\n");
    });
    
    // 步骤15：更新堆叠卡牌以反映新的顶部暴露状态
    if (_stackView)
    {
        _stackView->refreshStackCardDisplays();
    }
}

StackView* StackController::getStackView() const
{
    return _stackView;
}

