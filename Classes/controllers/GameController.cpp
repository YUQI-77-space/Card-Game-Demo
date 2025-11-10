#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "managers/GameMemory.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

GameController* GameController::create()
{
    GameController* ret = new (std::nothrow) GameController();
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

bool GameController::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    _gameModel = nullptr;
    _gameView = nullptr;
    _stackController = nullptr;
    _playfieldController = nullptr;
    _gameMemory = nullptr;
    _undoButton = nullptr;
    
    return true;
}

GameController::~GameController()
{
    if (_gameModel)
    {
        delete _gameModel;
        _gameModel = nullptr;
    }
    
    if (_stackController)
    {
        delete _stackController;
        _stackController = nullptr;
    }
    
    if (_playfieldController)
    {
        delete _playfieldController;
        _playfieldController = nullptr;
    }
    
    if (_gameMemory)
    {
        delete _gameMemory;
        _gameMemory = nullptr;
    }
}

bool GameController::startGame(int levelId)
{
    // 步骤1：加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig)
    {
        CCLOG("加载关卡配置失败，关卡ID: %d", levelId);
        return false;
    }
    
    // 步骤2：生成运行时游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    delete levelConfig;  // 使用后释放配置数据
    
    if (!_gameModel)
    {
        CCLOG("生成游戏模型失败");
        return false;
    }
    
    // 步骤3：初始化游戏内存管理器
    _gameMemory = new GameMemory(_gameModel);
    _gameMemory->setRollbackAnimationCallback([this](const RoundState* prevRound, const RoundState* currRound) {
        this->_handleRollbackAnimation(prevRound, currRound);
    });
    
    // 步骤4：初始化核心视图
    _initView();
    
    // 步骤5：初始化堆叠控制器
    _stackController = new StackController(_gameModel, _gameMemory);
    _stackController->init();
    _stackController->initView(_gameView->getStackContainer());
    
    // 步骤6：初始化游戏区域控制器
    _playfieldController = new PlayFieldController(_gameModel, _gameMemory);
    _playfieldController->setStackController(_stackController);
    _playfieldController->init();
    _playfieldController->initView(_gameView->getPlayfieldContainer());
    
    // 步骤7：初始化第一个回合状态
    _gameMemory->initializeFirstRound();
    
    // 步骤8：创建撤销按钮
    _createUndoButton();
    
    CCLOG("游戏成功启动，关卡ID: %d", levelId);
    return true;
}

void GameController::_initView()
{
    // 创建主游戏视图
    _gameView = GameView::create();
    this->addChild(_gameView);
}

void GameController::_createUndoButton()
{
    // 创建撤销按钮控件
    _undoButton = cocos2d::ui::Button::create();
    _undoButton->setTitleText("回退");
    _undoButton->setTitleFontSize(48);
    _undoButton->setTitleColor(Color3B::WHITE);
    
    // 将按钮定位在底部中心（水平居中）
    _undoButton->setPosition(Vec2(540, 150));
    
    // 配置按钮大小和外观
    _undoButton->setScale9Enabled(true);
    _undoButton->setContentSize(Size(200, 100));
    _undoButton->setColor(Color3B(70, 130, 180));  // 钢蓝色
    
    // 通过透明度添加圆角效果
    _undoButton->setOpacity(230);
    
    // 注册点击事件处理器
    _undoButton->addClickEventListener([this](Ref* sender) {
        this->_handleUndoButtonClick();
    });
    
    this->addChild(_undoButton, 100);
    
    CCLOG("撤销按钮已创建");
}

void GameController::_handleUndoButtonClick()
{
    if (!_gameMemory)
    {
        return;
    }
    
    if (_gameMemory->canGoBack())
    {
        CCLOG("撤销按钮被点击 - 回退一个回合");
        _gameMemory->goBackOneRound();
    }
    else
    {
        CCLOG("无法撤销：没有上一个回合可恢复");
    }
}

void GameController::_handleRollbackAnimation(const RoundState* previousRound, const RoundState* currentRound)
{
    if (!previousRound || !currentRound || !_gameView)
    {
        return;
    }
    
    CCLOG("=== 回滚动画开始 ===");
    CCLOG("从回合 %d 回滚到回合 %d", 
          currentRound->getRoundNumber(), previousRound->getRoundNumber());
    
    StackView* stackView = _stackController ? _stackController->getStackView() : nullptr;
    PlayFieldView* playfieldView = _playfieldController ? _playfieldController->getPlayFieldView() : nullptr;
    
    if (!stackView || !playfieldView)
    {
        CCLOG("错误：未找到视图！");
        return;
    }
    
    // 步骤1：在状态改变之前获取当前顶部托盘卡牌
    CardView* movingCardView = stackView->getTopTrayCardView();
    int movingCardId = movingCardView ? movingCardView->getCardModel()->getCardId() : -1;
    cocos2d::Vec2 startPosition = movingCardView ? movingCardView->getPosition() : cocos2d::Vec2::ZERO;
    
    CCLOG("当前托盘卡牌: ID=%d 位置 (%.0f, %.0f)", movingCardId, startPosition.x, startPosition.y);
    
    // 获取恢复后的游戏状态
    const std::vector<CardModel*>& handCards = _gameModel->getHandStackCards();
    const std::vector<CardModel*>& altCards = _gameModel->getStackCards();
    const std::vector<CardModel*>& playfieldCards = _gameModel->getPlayfieldCards();
    
    CCLOG("目标状态: 手牌=%d, 备用堆叠=%d, 游戏区域=%d", 
          (int)handCards.size(), (int)altCards.size(), (int)playfieldCards.size());
    
    // 步骤2：确定移动卡牌应该去的位置
    bool cardGoesToAlt = false;
    bool cardGoesToPlayfield = false;
    cocos2d::Vec2 targetPosition;
    // 跟踪备用堆叠的目标信息，以便稍后计算视图对齐位置
    int movingAltIndex = -1;
    CardModel* movingAltCardModel = nullptr;
    
    if (movingCardId != -1)
    {
        // 检查备用堆叠
        for (int i = 0; i < (int)altCards.size(); ++i)
        {
            CardModel* card = altCards[i];
            if (card && card->getCardId() == movingCardId)
            {
                cardGoesToAlt = true;
                movingAltIndex = i;
                movingAltCardModel = card;
                CCLOG("卡牌 %d 将移回备用堆叠（索引 %d）", 
                      movingCardId, movingAltIndex);
                break;
            }
        }
        
        // 检查游戏区域
        if (!cardGoesToAlt)
        {
            for (CardModel* card : playfieldCards)
            {
                if (card->getCardId() == movingCardId)
                {
                    cardGoesToPlayfield = true;
                    targetPosition = card->getPosition();
                    CCLOG("卡牌 %d 将移动到游戏区域位置 (%.0f, %.0f)", 
                          movingCardId, targetPosition.x, targetPosition.y);
                    break;
                }
            }
        }
    }
    
    // 步骤3：预渲染目标区域卡牌以避免"透视"效果
    if (movingCardView && (cardGoesToAlt || cardGoesToPlayfield))
    {
        CCLOG("预渲染目标区域...");
        
        // 保持移动卡牌视图，但暂时分离
        movingCardView->retain();
        movingCardView->removeFromParent();
        
        // 重建备用堆叠（排除移动卡牌）
        stackView->clearAllStackCards();
        for (CardModel* card : altCards)
        {
            if (card->getCardId() != movingCardId)  // 跳过移动卡牌
            {
                CardView* cardView = CardView::create(card);
                if (cardView)
                {
                    stackView->addStackCardView(cardView);
                }
            }
        }
        
        // 重建游戏区域（排除移动卡牌）
        playfieldView->clearAllCards();
        for (CardModel* card : playfieldCards)
        {
            if (card->getCardId() != movingCardId)  // 跳过移动卡牌
            {
                CardView* cardView = CardView::create(card);
                if (cardView)
                {
                    playfieldView->addCardView(cardView);
                }
            }
        }
        
        // 将移动卡牌添加回场景，使用高z-order
        if (cardGoesToAlt)
        {
            stackView->addChild(movingCardView, 10000);  // 顶层
        }
        else if (cardGoesToPlayfield)
        {
            playfieldView->addChild(movingCardView, 10000);  // 顶层
        }
        
        // 确保移动卡牌在起始位置
        movingCardView->setPosition(startPosition);
        movingCardView->release();
        
        // 如果移动到备用堆叠，计算与当前堆叠布局对齐的最终目标位置
        if (cardGoesToAlt)
        {
            float baseX = 0.0f;
            float baseY = 0.0f;
            float offsetX = 0.0f;
            int found = 0;
            
            // 查找前两个可见的备用堆叠卡牌视图以推导基点和偏移
            for (CardModel* card : altCards)
            {
                if (!card || card->getCardId() == movingCardId)
                {
                    continue;
                }
                CardView* view = stackView->getCardViewById(card->getCardId());
                if (view)
                {
                    if (found == 0)
                    {
                        baseX = view->getPosition().x;
                        baseY = view->getPosition().y;
                    }
                    else if (found == 1)
                    {
                        offsetX = view->getPosition().x - baseX;
                        break;
                    }
                    found++;
                }
            }
            
            // 如果卡牌不足以推导布局，使用备用方案
            if (found == 0)
            {
                // 没有其他备用卡牌；回退到模型存储的基点位置
                if (movingAltCardModel)
                {
                    targetPosition = movingAltCardModel->getPosition();
                }
            }
            else
            {
                if (found == 1)
                {
                    // 只有一个参考；如果无法推导，假设默认间距
                    if (offsetX == 0.0f)
                    {
                        offsetX = 50.0f; // StackView使用的默认水平间距
                    }
                }
                // 使用推导的基点和间距以及原始索引计算目标位置
                targetPosition = cocos2d::Vec2(baseX + offsetX * movingAltIndex, baseY);
            }
        }
        
        // 步骤4：播放移动动画
        CCLOG("播放动画: (%.0f, %.0f) -> (%.0f, %.0f)", 
              startPosition.x, startPosition.y, targetPosition.x, targetPosition.y);
        
        movingCardView->playMoveAnimation(targetPosition, [=]() {
            CCLOG("回滚动画完成！");
            
            // 步骤5：最终重建以完成回滚
            this->_finalizeRollback();
        });
    }
    else
    {
        // 不需要动画，直接重建
        CCLOG("不需要动画，直接重建...");
        _finalizeRollback();
    }
}

void GameController::_finalizeRollback()
{
    CCLOG("完成回滚 - 完全重建...");
    
    StackView* stackView = _stackController ? _stackController->getStackView() : nullptr;
    PlayFieldView* playfieldView = _playfieldController ? _playfieldController->getPlayFieldView() : nullptr;
    
    if (!stackView || !playfieldView)
    {
        return;
    }
    
    // 重建手牌/托盘
    const std::vector<CardModel*>& handCards = _gameModel->getHandStackCards();
    if (!handCards.empty())
    {
        CardModel* topHandCard = handCards.back();
        CardView* topHandView = CardView::create(topHandCard);
        if (topHandView)
        {
            stackView->setTopTrayCardView(topHandView);
        }
    }
    
    // 完全重建备用堆叠
    stackView->clearAllStackCards();
    const std::vector<CardModel*>& altCards = _gameModel->getStackCards();
    for (CardModel* card : altCards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            stackView->addStackCardView(cardView);
        }
    }
    
    // 完全重建游戏区域
    playfieldView->clearAllCards();
    const std::vector<CardModel*>& playfieldCards = _gameModel->getPlayfieldCards();
    for (CardModel* card : playfieldCards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            playfieldView->addCardView(cardView);
        }
    }
    
    // 刷新显示
    stackView->refreshStackCardDisplays();
    playfieldView->refreshCardDisplays();
    
    CCLOG("=== 回滚完成 ===\n");
}

