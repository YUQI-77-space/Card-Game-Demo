#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "utils/GameTypes.h"
#include <string>

/**
 * @class CardResConfig
 * @brief 卡牌资源配置辅助类
 *
 * 职责说明：
 * - 提供卡牌资源的路径
 * - 返回适合的花色和面值组合的纹理
 * - 支持复合卡牌渲染（背景 + 花色 + 面值）
 */
class CardResConfig
{
public:
    /**
     * @brief 获取卡牌背景纹理路径
     * @return 背景纹理的相对路径
     */
    static std::string getCardBackgroundPath();

    /**
     * @brief 获取卡牌背面纹理路径
     * @return 卡牌背面纹理的相对路径
     */
    static std::string getCardBackPath();

    /**
     * @brief 获取花色图标纹理路径
     * @param suit 花色枚举值
     * @return 花色图标纹理的相对路径
     */
    static std::string getSuitIconPath(CardSuitType suit);

    /**
     * @brief 获取大号数字纹理路径（中心显示）
     * @param face 面值枚举
     * @param suit 花色枚举（用于确定颜色）
     * @return 大号数字纹理的相对路径
     */
    static std::string getBigNumberPath(CardFaceType face, CardSuitType suit);

    /**
     * @brief 获取小号数字纹理路径（角落显示）
     * @param face 面值枚举
     * @param suit 花色枚举（用于确定颜色）
     * @return 小号数字纹理的相对路径
     */
    static std::string getSmallNumberPath(CardFaceType face, CardSuitType suit);

private:
    /**
     * @brief 判断花色是否为红色
     * @param suit 花色枚举值
     * @return 红色花色返回true，否则返回false
     */
    static bool isRedSuit(CardSuitType suit);

    /**
     * @brief 将面值转换为字符串表示
     * @param face 面值枚举
     * @return 面值字符串（A, 2-10, J, Q, K）
     */
    static std::string getFaceString(CardFaceType face);
};

#endif // __CARD_RES_CONFIG_H__

