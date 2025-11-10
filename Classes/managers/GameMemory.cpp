#include "GameMemory.h"
#include "cocos2d.h"

USING_NS_CC;

GameMemory::GameMemory(GameModel* gameModel)
    : _gameModel(gameModel)
    , _currentRoundIndex(-1)
{
}

GameMemory::~GameMemory()
{
    clear();
}

void GameMemory::initializeFirstRound()
{
    // 捕获初始游戏状态作为回合0
    RoundState* initialState = captureCurrentState();
    if (initialState)
    {
        initialState->setRoundNumber(0);
        _roundHistory.push_back(initialState);
        _currentRoundIndex = 0;
        CCLOG("GameMemory: 已初始化第一个回合（回合 0）");
    }
}

int GameMemory::saveCurrentRound()
{
    if (!_gameModel)
    {
        return -1;
    }

    // 捕获当前状态
    RoundState* newRound = captureCurrentState();
    if (!newRound)
    {
        return -1;
    }

    // 如果不在最新回合，丢弃未来的回合
    while (_currentRoundIndex < (int)_roundHistory.size() - 1)
    {
        RoundState* discarded = _roundHistory.back();
        _roundHistory.pop_back();
        delete discarded;
    }

    // 添加新回合
    int newRoundNumber = _roundHistory.size();
    newRound->setRoundNumber(newRoundNumber);
    _roundHistory.push_back(newRound);
    _currentRoundIndex = newRoundNumber;

    CCLOG("GameMemory: 已保存回合 %d（总回合数: %d）", newRoundNumber, (int)_roundHistory.size());
    return newRoundNumber;
}

bool GameMemory::goBackOneRound()
{
    if (!canGoBack())
    {
        CCLOG("GameMemory: 无法回退 - 没有上一个回合");
        return false;
    }

    // 获取上一个回合
    int previousRoundIndex = _currentRoundIndex - 1;
    const RoundState* previousRound = _roundHistory[previousRoundIndex];
    const RoundState* currentRound = _roundHistory[_currentRoundIndex];

    CCLOG("GameMemory: 从回合 %d 回滚到回合 %d", _currentRoundIndex, previousRoundIndex);

    // 重要：先恢复状态，然后触发动画回调
    // 回调需要从恢复的模型状态中读取
    restoreState(previousRound);
    _currentRoundIndex = previousRoundIndex;

    // 恢复状态后触发动画回调
    if (_rollbackAnimationCallback)
    {
        _rollbackAnimationCallback(previousRound, currentRound);
    }

    CCLOG("GameMemory: 已回滚到回合 %d", _currentRoundIndex);
    return true;
}

bool GameMemory::canGoBack() const
{
    return _currentRoundIndex > 0;
}

int GameMemory::getCurrentRoundNumber() const
{
    return _currentRoundIndex;
}

int GameMemory::getRoundCount() const
{
    return _roundHistory.size();
}

void GameMemory::clear()
{
    for (RoundState* round : _roundHistory)
    {
        delete round;
    }
    _roundHistory.clear();
    _currentRoundIndex = -1;
}

void GameMemory::setRollbackAnimationCallback(const std::function<void(const RoundState*, const RoundState*)>& callback)
{
    _rollbackAnimationCallback = callback;
}

RoundState* GameMemory::captureCurrentState()
{
    if (!_gameModel)
    {
        return nullptr;
    }

    RoundState* state = new RoundState();

    // 捕获手牌堆叠
    const std::vector<CardModel*>& handCards = _gameModel->getHandStackCards();
    for (CardModel* card : handCards)
    {
        if (card)
        {
            state->getHandStack().pushCard(card->getCardId(), card->getPosition());
        }
    }

    // 捕获备用堆叠
    const std::vector<CardModel*>& altCards = _gameModel->getStackCards();
    for (CardModel* card : altCards)
    {
        if (card)
        {
            state->getAlternativeStack().pushCard(card->getCardId(), card->getPosition());
        }
    }

    // 捕获主堆叠（游戏区域）
    const std::vector<CardModel*>& mainCards = _gameModel->getPlayfieldCards();
    for (CardModel* card : mainCards)
    {
        if (card)
        {
            state->getMainStack().pushCard(card->getCardId(), card->getPosition());
        }
    }

    return state;
}

void GameMemory::restoreState(const RoundState* roundState)
{
    if (!_gameModel || !roundState)
    {
        return;
    }

    CCLOG("=== GameMemory: 恢复状态 ===");
    CCLOG("目标回合: %d", roundState->getRoundNumber());
    
    // 清除现有堆叠
    CCLOG("清除现有堆叠...");
    _gameModel->getPlayfieldCards().clear();
    _gameModel->getStackCards().clear();
    _gameModel->getHandStackCards().clear();
    
    // 恢复手牌堆叠
    const std::vector<CardInStack>& handCards = roundState->getHandStack().getCards();
    CCLOG("恢复手牌堆叠: %d 张卡牌", (int)handCards.size());
    for (size_t i = 0; i < handCards.size(); i++)
    {
        const CardInStack& cardInStack = handCards[i];
        CardModel* card = _gameModel->getCardById(cardInStack.cardId);
        if (card)
        {
            card->setPosition(cardInStack.position);
            _gameModel->getHandStackCards().push_back(card);
            CCLOG("  [%d] 卡牌 ID=%d 在 (%.0f, %.0f)", 
                  (int)i, cardInStack.cardId, 
                  cardInStack.position.x, cardInStack.position.y);
        }
        else
        {
            CCLOG("  [%d] 错误：未找到卡牌 ID=%d！", (int)i, cardInStack.cardId);
        }
    }
    
    // 恢复备用堆叠
    const std::vector<CardInStack>& altCards = roundState->getAlternativeStack().getCards();
    CCLOG("恢复备用堆叠: %d 张卡牌", (int)altCards.size());
    for (size_t i = 0; i < altCards.size(); i++)
    {
        const CardInStack& cardInStack = altCards[i];
        CardModel* card = _gameModel->getCardById(cardInStack.cardId);
        if (card)
        {
            card->setPosition(cardInStack.position);
            _gameModel->getStackCards().push_back(card);
            CCLOG("  [%d] 卡牌 ID=%d 在 (%.0f, %.0f)", 
                  (int)i, cardInStack.cardId, 
                  cardInStack.position.x, cardInStack.position.y);
        }
        else
        {
            CCLOG("  [%d] 错误：未找到卡牌 ID=%d！", (int)i, cardInStack.cardId);
        }
    }
    
    // 恢复主堆叠（游戏区域）
    const std::vector<CardInStack>& mainCards = roundState->getMainStack().getCards();
    CCLOG("恢复主堆叠（游戏区域）: %d 张卡牌", (int)mainCards.size());
    for (size_t i = 0; i < mainCards.size(); i++)
    {
        const CardInStack& cardInStack = mainCards[i];
        CardModel* card = _gameModel->getCardById(cardInStack.cardId);
        if (card)
        {
            card->setPosition(cardInStack.position);
            _gameModel->getPlayfieldCards().push_back(card);
            CCLOG("  [%d] 卡牌 ID=%d 在 (%.0f, %.0f)", 
                  (int)i, cardInStack.cardId, 
                  cardInStack.position.x, cardInStack.position.y);
        }
        else
        {
            CCLOG("  [%d] 错误：未找到卡牌 ID=%d！", (int)i, cardInStack.cardId);
        }
    }
    
    // 更新可见性状态
    CCLOG("更新可见性状态...");
    _gameModel->updatePlayfieldVisibility();
    _gameModel->updateStackVisibility();
    
    CCLOG("=== 状态恢复完成 ===\n");
}

