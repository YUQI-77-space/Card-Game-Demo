#include "LevelConfig.h"

LevelConfig::LevelConfig()
    : _levelId(0)
{
}

LevelConfig::~LevelConfig()
{
}

const std::vector<CardConfig>& LevelConfig::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardConfig>& LevelConfig::getStackCards() const
{
    return _stackCards;
}

int LevelConfig::getLevelId() const
{
    return _levelId;
}

void LevelConfig::setPlayfieldCards(const std::vector<CardConfig>& cards)
{
    _playfieldCards = cards;
}

void LevelConfig::setStackCards(const std::vector<CardConfig>& cards)
{
    _stackCards = cards;
}

void LevelConfig::setLevelId(int levelId)
{
    _levelId = levelId;
}

