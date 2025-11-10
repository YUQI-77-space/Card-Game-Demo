#include "PlayFieldController.h"
#include "StackController.h"

USING_NS_CC;

PlayFieldController::PlayFieldController(GameModel* gameModel, GameMemory* gameMemory)
    : _gameModel(gameModel)
    , _playfieldView(nullptr)
    , _gameMemory(gameMemory)
    , _stackController(nullptr)
{
}

PlayFieldController::~PlayFieldController()
{
    // _playfieldView由Cocos2d-x管理，无需手动删除
}

bool PlayFieldController::init()
{
    if (!_gameModel)
    {
        return false;
    }
    
    return true;
}

void PlayFieldController::initView(cocos2d::Node* parent)
{
    if (!parent || !_gameModel)
    {
        return;
    }
    
    // 创建游戏区域视图
    _playfieldView = PlayFieldView::create();
    parent->addChild(_playfieldView);
    
    // 注册卡牌点击回调函数
    _playfieldView->setOnCardClickCallback([this](int cardId) {
        this->handleCardClick(cardId);
    });
    
    // 为游戏区域创建卡牌视图
    const std::vector<CardModel*>& playfieldCards = _gameModel->getPlayfieldCards();
    for (CardModel* card : playfieldCards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            _playfieldView->addCardView(cardView);
        }
    }

    // 刷新游戏区域卡牌显示
    if (_playfieldView)
    {
        _playfieldView->refreshCardDisplays();
    }
}

void PlayFieldController::handleCardClick(int cardId)
{
    CCLOG("游戏区域卡牌被点击: %d", cardId);
    
    // 检查卡牌是否可以与托盘卡牌匹配
    if (!checkCanMatch(cardId))
    {
        CCLOG("卡牌 %d 无法与顶部托盘卡牌匹配", cardId);
        return;
    }
    
    // 匹配成功时执行托盘替换
    replaceTrayWithPlayFieldCard(cardId);
}

bool PlayFieldController::checkCanMatch(int cardId)
{
    if (!_gameModel)
    {
        return false;
    }
    
    // 获取被点击的卡牌
    CardModel* clickedCard = _gameModel->getCardById(cardId);
    if (!clickedCard)
    {
        return false;
    }
    
    // 获取当前托盘卡牌
    CardModel* topTrayCard = _gameModel->getTopTrayCard();
    if (!topTrayCard)
    {
        return false;
    }
    
    // 检查卡牌是否可以匹配（点数差值为1）
    return clickedCard->canMatchWith(topTrayCard);
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int cardId)
{
    if (!_gameModel || !_playfieldView)
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
    
    // 步骤3：获取关联的卡牌视图
    CardView* clickedCardView = _playfieldView->getCardViewById(cardId);
    if (!clickedCardView)
    {
        return;
    }
    
    // 步骤4：在状态改变之前缓存起始位置和目标位置
    cocos2d::Vec2 fromPos = clickedCard->getPosition();
    cocos2d::Vec2 targetPos = currentTopCard->getPosition();
    
    CCLOG("=== 游戏区域匹配动画开始 ===");
    CCLOG("移动卡牌 %d 从 (%.0f, %.0f) 到 (%.0f, %.0f)", 
          clickedCard->getCardId(), fromPos.x, fromPos.y, targetPos.x, targetPos.y);
    
    // 步骤5：确保移动的卡牌在托盘中显示正面
    clickedCard->setFlipped(true);
    clickedCardView->updateDisplay();
    
    // 步骤6：确保卡牌从正确位置开始
    clickedCardView->setPosition(fromPos);
    
    // 步骤7：将卡牌提升到顶层以便在其他卡牌上流畅动画
    clickedCardView->setLocalZOrder(10000);
    
    // 步骤8：播放移动动画 - 完成从A到B的移动
    clickedCardView->playMoveAnimation(targetPos, [this, clickedCard, targetPos, clickedCardView]() {
        CCLOG("动画完成，更新模型状态...");
        
        // 步骤9：动画完成后更新模型状态
        clickedCard->setPosition(targetPos);
        
        // 步骤10：从游戏区域数据列表中移除卡牌
        _gameModel->removePlayfieldCard(clickedCard->getCardId());
        
        // 步骤11：将卡牌推入手牌堆叠（成为新的顶部卡牌）
        _gameModel->pushHandStackCard(clickedCard);
        
        // 步骤12：从游戏区域移除卡牌视图
        _playfieldView->removeCardView(clickedCardView);

        // 步骤13：刷新剩余的游戏区域卡牌以显示任何新暴露的卡牌
        _playfieldView->refreshCardDisplays();

        // 步骤14：将移动的卡牌附加到托盘
        if (_stackController && _stackController->getStackView())
        {
            _stackController->getStackView()->setTopTrayCardView(clickedCardView);
            _stackController->getStackView()->refreshStackCardDisplays();
        }
        
        CCLOG("已匹配并将卡牌 %d 推入手牌堆叠", clickedCard->getCardId());
        
        // 步骤15：所有操作完成后保存新的回合状态
        if (_gameMemory)
        {
            _gameMemory->saveCurrentRound();
        }

        // 步骤16：状态更新后检查胜利条件
        _checkGameVictory();
        
        CCLOG("=== 游戏区域匹配完成 ===\n");
    });
}

PlayFieldView* PlayFieldController::getPlayFieldView() const
{
    return _playfieldView;
}

void PlayFieldController::setStackController(StackController* stackController)
{
    _stackController = stackController;
}

void PlayFieldController::_checkGameVictory()
{
    if (!_gameModel)
    {
        return;
    }

    if (_gameModel->getPlayfieldCards().empty())
    {
        CCLOG("=== 游戏胜利：主牌堆已清空 ===");
        _showVictoryMessage();
    }
}

void PlayFieldController::_showVictoryMessage()
{
    if (!_playfieldView)
    {
        return;
    }
    
    // 获取场景以添加胜利UI
    cocos2d::Scene* scene = cocos2d::Director::getInstance()->getRunningScene();
    if (!scene)
    {
        return;
    }
    
    // 创建半透明背景遮罩（全屏覆盖）
    auto overlay = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180), 1080, 2080);
    overlay->setPosition(cocos2d::Vec2::ZERO);
    scene->addChild(overlay, 1000);
    
    // 创建胜利标签
    auto victoryLabel = cocos2d::Label::createWithSystemFont(
        "游戏胜利",
        "Arial",
        80
    );
    victoryLabel->setPosition(cocos2d::Vec2(540, 1660));
    victoryLabel->setColor(cocos2d::Color3B::YELLOW);
    victoryLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
    overlay->addChild(victoryLabel);
    
    // 添加缩放动画以强调效果
    auto scaleUp = cocos2d::ScaleTo::create(0.3f, 1.2f);
    auto scaleDown = cocos2d::ScaleTo::create(0.3f, 1.0f);
    auto sequence = cocos2d::Sequence::create(scaleUp, scaleDown, nullptr);
    auto repeat = cocos2d::RepeatForever::create(sequence);
    victoryLabel->runAction(repeat);
    
    CCLOG("胜利消息已显示在屏幕上");
}

