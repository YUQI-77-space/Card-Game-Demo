#include "CardModel.h"
#include <cmath>

CardModel::CardModel(int id, CardFaceType face, CardSuitType suit)
    : _cardId(id)
    , _faceType(face)
    , _suitType(suit)
    , _position(cocos2d::Vec2::ZERO)
    , _isFlipped(true)  // 默认正面朝上
{
}

CardModel::~CardModel()
{
}

int CardModel::getCardId() const
{
    return _cardId;
}

CardFaceType CardModel::getFaceType() const
{
    return _faceType;
}

CardSuitType CardModel::getSuitType() const
{
    return _suitType;
}

cocos2d::Vec2 CardModel::getPosition() const
{
    return _position;
}

bool CardModel::isFlipped() const
{
    return _isFlipped;
}

void CardModel::setPosition(const cocos2d::Vec2& pos)
{
    _position = pos;
}

void CardModel::setFlipped(bool flipped)
{
    _isFlipped = flipped;
}

bool CardModel::canMatchWith(const CardModel* otherCard) const
{
    if (!otherCard)
    {
        return false;
    }
    
    // 计算面值的绝对差值
    int diff = std::abs(static_cast<int>(_faceType) - static_cast<int>(otherCard->getFaceType()));
    
    // 当差值为1时卡牌可以匹配，忽略花色
    return (diff == 1);
}

