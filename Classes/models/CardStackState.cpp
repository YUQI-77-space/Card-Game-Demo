#include "CardStackState.h"

CardStackState::CardStackState()
{
}

CardStackState::~CardStackState()
{
}

void CardStackState::pushCard(int cardId, const cocos2d::Vec2& position)
{
    _cards.push_back(CardInStack(cardId, position));
}

bool CardStackState::popCard(int& outCardId, cocos2d::Vec2& outPosition)
{
    if (_cards.empty())
    {
        return false;
    }
    
    const CardInStack& top = _cards.back();
    outCardId = top.cardId;
    outPosition = top.position;
    _cards.pop_back();
    return true;
}

bool CardStackState::peekTop(int& outCardId, cocos2d::Vec2& outPosition) const
{
    if (_cards.empty())
    {
        return false;
    }
    
    const CardInStack& top = _cards.back();
    outCardId = top.cardId;
    outPosition = top.position;
    return true;
}

size_t CardStackState::getCardCount() const
{
    return _cards.size();
}

bool CardStackState::isEmpty() const
{
    return _cards.empty();
}

const std::vector<CardInStack>& CardStackState::getCards() const
{
    return _cards;
}

void CardStackState::clear()
{
    _cards.clear();
}

void CardStackState::setCards(const std::vector<CardInStack>& cards)
{
    _cards = cards;
}

