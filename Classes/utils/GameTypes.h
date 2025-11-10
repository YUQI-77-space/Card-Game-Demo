#ifndef __GAME_TYPES_H__
#define __GAME_TYPES_H__

/**
 * @file GameTypes.h
 * @brief 基本的卡牌相关类型定义
 */

// 卡牌花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红心
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 卡牌面值类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,        // A
    CFT_TWO,        // 2
    CFT_THREE,      // 3
    CFT_FOUR,       // 4
    CFT_FIVE,       // 5
    CFT_SIX,        // 6
    CFT_SEVEN,      // 7
    CFT_EIGHT,      // 8
    CFT_NINE,       // 9
    CFT_TEN,        // 10
    CFT_JACK,       // J
    CFT_QUEEN,      // Q
    CFT_KING,       // K
    CFT_NUM_CARD_FACE_TYPES
};

#endif // __GAME_TYPES_H__

