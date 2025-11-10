#include "CardResConfig.h"

std::string CardResConfig::getCardBackgroundPath()
{
    return "res/card_general.png";
}

std::string CardResConfig::getCardBackPath()
{
    return "res/card_general.png";
}

std::string CardResConfig::getSuitIconPath(CardSuitType suit)
{
    switch (suit)
    {
        case CST_CLUBS:
            return "res/suits/club.png";
        case CST_DIAMONDS:
            return "res/suits/diamond.png";
        case CST_HEARTS:
            return "res/suits/heart.png";
        case CST_SPADES:
            return "res/suits/spade.png";
        default:
            return "";
    }
}

std::string CardResConfig::getBigNumberPath(CardFaceType face, CardSuitType suit)
{
    // 根据花色确定颜色前缀（红色或黑色）
    std::string colorPrefix = isRedSuit(suit) ? "big_red_" : "big_black_";
    std::string faceStr = getFaceString(face);
    
    if (faceStr.empty())
    {
        return "";
    }
    
    return "res/number/" + colorPrefix + faceStr + ".png";
}

std::string CardResConfig::getSmallNumberPath(CardFaceType face, CardSuitType suit)
{
    // 根据花色确定颜色前缀（红色或黑色）
    std::string colorPrefix = isRedSuit(suit) ? "small_red_" : "small_black_";
    std::string faceStr = getFaceString(face);
    
    if (faceStr.empty())
    {
        return "";
    }
    
    return "res/number/" + colorPrefix + faceStr + ".png";
}

bool CardResConfig::isRedSuit(CardSuitType suit)
{
    // 判断花色是否为红色（方块和红心为红色）
    return (suit == CST_DIAMONDS || suit == CST_HEARTS);
}

std::string CardResConfig::getFaceString(CardFaceType face)
{
    switch (face)
    {
        case CFT_ACE:
            return "A";
        case CFT_TWO:
            return "2";
        case CFT_THREE:
            return "3";
        case CFT_FOUR:
            return "4";
        case CFT_FIVE:
            return "5";
        case CFT_SIX:
            return "6";
        case CFT_SEVEN:
            return "7";
        case CFT_EIGHT:
            return "8";
        case CFT_NINE:
            return "9";
        case CFT_TEN:
            return "10";
        case CFT_JACK:
            return "J";
        case CFT_QUEEN:
            return "Q";
        case CFT_KING:
            return "K";
        default:
            return "";
    }
}

