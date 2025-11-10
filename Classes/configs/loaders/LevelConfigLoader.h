#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "configs/models/LevelConfig.h"
#include <string>

/**
 * @class LevelConfigLoader
 * @brief 负责解析关卡配置文件的加载器
 *
 * 职责说明：
 * - 从JSON文件加载关卡数据
 * - 将解析的JSON转换为LevelConfig对象
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 从JSON加载关卡配置
     * @param levelId 关卡标识符
     * @return LevelConfig实例的指针，失败时返回nullptr
     */
    static LevelConfig* loadLevelConfig(int levelId);

private:
    /**
     * @brief 从JSON数据解析卡牌面值
     * @param faceValue JSON中的数字面值
     * @return 对应的CardFaceType
     */
    static CardFaceType parseFaceType(int faceValue);

    /**
     * @brief 从JSON数据解析卡牌花色
     * @param suitValue JSON中的数字花色值
     * @return 对应的CardSuitType
     */
    static CardSuitType parseSuitType(int suitValue);
};

#endif // __LEVEL_CONFIG_LOADER_H__

