#include "GameModel.h"
#include <cmath>

GameModel::GameModel()
    : _nextCardId(1)
{
}

GameModel::~GameModel()
{
    clear();
}

void GameModel::addPlayfieldCard(CardModel* card)
{
    if (card)
    {
        _playfieldCards.push_back(card);
        _cardMap[card->getCardId()] = card;
        // 可见性将在所有卡牌添加后更新
    }
}

void GameModel::addStackCard(CardModel* card)
{
    if (card)
    {
        _stackCards.push_back(card);
        _cardMap[card->getCardId()] = card;
        // 可见性将在所有卡牌添加后更新
    }
}

CardModel* GameModel::getCardById(int cardId) const
{
    auto it = _cardMap.find(cardId);
    if (it != _cardMap.end())
    {
        return it->second;
    }
    return nullptr;
}

CardModel* GameModel::getTopTrayCard() const
{
    if (_handStackCards.empty())
    {
        return nullptr;
    }
    return _handStackCards.back();
}

void GameModel::pushHandStackCard(CardModel* card)
{
    if (card)
    {
        _handStackCards.push_back(card);
        card->setFlipped(true);
        CCLOG("GameModel: 已将卡牌 %d 推入手牌堆叠（总数: %d）", 
              card->getCardId(), (int)_handStackCards.size());
    }
}

const std::vector<CardModel*>& GameModel::getHandStackCards() const
{
    return _handStackCards;
}

const std::vector<CardModel*>& GameModel::getPlayfieldCards() const
{
    return _playfieldCards;
}

std::vector<CardModel*>& GameModel::getPlayfieldCards()
{
    return _playfieldCards;
}

const std::vector<CardModel*>& GameModel::getStackCards() const
{
    return _stackCards;
}

std::vector<CardModel*>& GameModel::getStackCards()
{
    return _stackCards;
}

std::vector<CardModel*>& GameModel::getHandStackCards()
{
    return _handStackCards;
}

void GameModel::removePlayfieldCard(int cardId)
{
    for (auto it = _playfieldCards.begin(); it != _playfieldCards.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            _playfieldCards.erase(it);
            break;
        }
    }

    updatePlayfieldVisibility();
}

void GameModel::removeStackCard(int cardId)
{
    for (auto it = _stackCards.begin(); it != _stackCards.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            _stackCards.erase(it);
            break;
        }
    }

    updateStackVisibility();
}

CardModel* GameModel::popStackCard()
{
    if (_stackCards.empty())
    {
        return nullptr;
    }
    
    CardModel* card = _stackCards.back();
    _stackCards.pop_back();
    updateStackVisibility();
    return card;
}

int GameModel::getNextCardId()
{
    return _nextCardId++;
}

void GameModel::clear()
{
    // 删除所有卡牌对象！
    for (auto& pair : _cardMap)
    {
        delete pair.second;
    }
    
    _cardMap.clear();
    _playfieldCards.clear();
    _stackCards.clear();
    _handStackCards.clear();
    _nextCardId = 1;
}

void GameModel::updatePlayfieldVisibility()
{
    // 列阈值：判断卡牌是否在同一列（水平距离）
    static const float kColumnThreshold = 120.0f;
    // 垂直阈值：判断卡牌是否被覆盖（垂直距离）
    static const float kVerticalThreshold = 40.0f;
    
    CCLOG("=== 更新游戏区域可见性 ===");
    
    // 遍历所有游戏区域卡牌
    for (CardModel* card : _playfieldCards)
    {
        if (!card)
        {
            continue;
        }
        
        // 检查当前卡牌是否被其他卡牌覆盖
        bool covered = false;
        for (CardModel* other : _playfieldCards)
        {
            if (!other || other == card)
            {
                continue;
            }
            
            // 计算水平距离和垂直距离
            float dx = std::fabs(other->getPosition().x - card->getPosition().x);
            float dy = other->getPosition().y - card->getPosition().y;
            
            // 如果水平距离小于阈值且垂直距离大于阈值，说明被覆盖
            if (dx <= kColumnThreshold && dy > kVerticalThreshold)
            {
                CCLOG("卡牌 %d 在 (%.0f, %.0f) 被卡牌 %d 在 (%.0f, %.0f) 覆盖", 
                      card->getCardId(), card->getPosition().x, card->getPosition().y,
                      other->getCardId(), other->getPosition().x, other->getPosition().y);
                covered = true;
                break;
            }
        }
        
        // 设置卡牌是否正面朝上（未被覆盖则正面朝上）
        card->setFlipped(!covered);
        CCLOG("卡牌 %d 在 (%.0f, %.0f) -> %s", 
              card->getCardId(), card->getPosition().x, card->getPosition().y,
              covered ? "背面" : "正面");
    }
    
    CCLOG("=== 游戏区域可见性更新完成 ===");
}

void GameModel::updateStackVisibility()
{
    for (CardModel* card : _stackCards)
    {
        if (card)
        {
            card->setFlipped(false);
        }
    }
    
    if (!_stackCards.empty())
    {
        CardModel* topCard = _stackCards.back();
        if (topCard)
        {
            topCard->setFlipped(true);
        }
    }
}

