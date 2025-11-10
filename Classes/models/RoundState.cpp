#include "RoundState.h"

RoundState::RoundState(int roundNumber)
    : _roundNumber(roundNumber)
{
}

RoundState::~RoundState()
{
}

int RoundState::getRoundNumber() const
{
    return _roundNumber;
}

void RoundState::setRoundNumber(int roundNumber)
{
    _roundNumber = roundNumber;
}

CardStackState& RoundState::getHandStack()
{
    return _handStack;
}

const CardStackState& RoundState::getHandStack() const
{
    return _handStack;
}

CardStackState& RoundState::getAlternativeStack()
{
    return _alternativeStack;
}

const CardStackState& RoundState::getAlternativeStack() const
{
    return _alternativeStack;
}

CardStackState& RoundState::getMainStack()
{
    return _mainStack;
}

const CardStackState& RoundState::getMainStack() const
{
    return _mainStack;
}

RoundState* RoundState::clone() const
{
    RoundState* newState = new RoundState(_roundNumber);
    newState->_handStack.setCards(_handStack.getCards());
    newState->_alternativeStack.setCards(_alternativeStack.getCards());
    newState->_mainStack.setCards(_mainStack.getCards());
    return newState;
}

