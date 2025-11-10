#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "utils/GameTypes.h"

/**
 * @class CardModel
 * @brief 表示单张卡牌的数据模型
 *
 * 职责说明：
 * - 存储卡牌的基本属性（面值、花色、位置等）
 * - 提供卡牌匹配逻辑
 * - 支持序列化风格的访问器
 */
class CardModel
{
private:
    int _cardId;                    // 唯一卡牌标识符
    CardFaceType _faceType;         // 卡牌面值（A, 2-10, J, Q, K）
    CardSuitType _suitType;         // 卡牌花色（梅花、方块、红心、黑桃）
    cocos2d::Vec2 _position;        // 屏幕上的位置
    bool _isFlipped;                // 卡牌是否正面朝上

public:
    /**
     * @brief 构造卡牌模型
     * @param id 卡牌标识符
     * @param face 卡牌面值
     * @param suit 卡牌花色
     */
    CardModel(int id, CardFaceType face, CardSuitType suit);

    /**
     * @brief 析构函数
     */
    ~CardModel();

    /**
     * @brief 获取卡牌标识符
     * @return 卡牌标识符
     */
    int getCardId() const;

    /**
     * @brief 获取面值
     * @return 卡牌面值
     */
    CardFaceType getFaceType() const;

    /**
     * @brief 获取花色
     * @return 卡牌花色
     */
    CardSuitType getSuitType() const;

    /**
     * @brief 获取卡牌位置
     * @return 位置向量
     */
    cocos2d::Vec2 getPosition() const;

    /**
     * @brief 检查卡牌是否正面朝上
     * @return 正面朝上返回true
     */
    bool isFlipped() const;

    /**
     * @brief 设置卡牌位置
     * @param pos 新位置
     */
    void setPosition(const cocos2d::Vec2& pos);

    /**
     * @brief 设置卡牌是否正面朝上
     * @param flipped 正面朝上为true
     */
    void setFlipped(bool flipped);

    /**
     * @brief 判断此卡牌是否可以与另一张卡牌匹配
     * @param otherCard 要比较的目标卡牌
     * @return 当面值差值为1时返回true
     */
    bool canMatchWith(const CardModel* otherCard) const;
};

#endif // __CARD_MODEL_H__

