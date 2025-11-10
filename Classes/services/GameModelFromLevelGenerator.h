#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

/**
 * @class GameModelFromLevelGenerator
 * @brief 将关卡配置转换为运行时模型的服务类
 *
 * 职责说明：
 * - 将静态LevelConfig数据转换为GameModel实例
 * - 为游戏区域和堆叠区域创建CardModel对象
 * - 提供无状态的辅助工具函数
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 从LevelConfig生成GameModel
     * @param levelConfig 关卡配置数据
     * @return 新创建的GameModel实例
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);

private:
    /**
     * @brief 创建游戏区域卡牌模型
     * @param config 关卡配置数据
     * @param gameModel 要填充的目标游戏模型
     */
    static void _createPlayfieldCards(const LevelConfig* config, GameModel* gameModel);

    /**
     * @brief 创建堆叠卡牌模型
     * @param config 关卡配置数据
     * @param gameModel 要填充的目标游戏模型
     */
    static void _createStackCards(const LevelConfig* config, GameModel* gameModel);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

