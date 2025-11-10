#ifndef __GAME_MEMORY_H__
#define __GAME_MEMORY_H__

#include "models/RoundState.h"
#include "models/GameModel.h"
#include <vector>
#include <functional>

/**
 * @class GameMemory
 * @brief 负责基于回合的游戏状态内存和回滚的管理器
 *
 * 职责说明：
 * - 存储每个回合的完整游戏状态
 * - 捕获当前游戏状态并保存为新回合
 * - 恢复之前的回合状态以进行回滚
 * - 提供回合导航（前进/后退）
 */
class GameMemory
{
private:
    std::vector<RoundState*> _roundHistory;                     // 所有已保存的回合
    int _currentRoundIndex;                                     // 当前活动回合
    GameModel* _gameModel;                                      // 游戏模型的引用
    std::function<void(const RoundState*, const RoundState*)> _rollbackAnimationCallback;  // 回滚动画的回调函数

public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏模型实例
     */
    GameMemory(GameModel* gameModel);

    /**
     * @brief 析构函数
     */
    ~GameMemory();

    /**
     * @brief 捕获当前游戏状态并保存为新回合
     * @return 已保存回合的回合号
     */
    int saveCurrentRound();

    /**
     * @brief 回退一个回合并恢复该状态
     * @return 如果回滚成功返回true，如果没有上一个回合则返回false
     */
    bool goBackOneRound();

    /**
     * @brief 检查是否可以回滚
     * @return 如果有上一个回合可恢复则返回true
     */
    bool canGoBack() const;

    /**
     * @brief 获取当前回合号
     * @return 回合号
     */
    int getCurrentRoundNumber() const;

    /**
     * @brief 获取已保存回合的总数
     * @return 回合数
     */
    int getRoundCount() const;

    /**
     * @brief 清除所有回合历史
     */
    void clear();

    /**
     * @brief 注册回滚动画的回调函数
     * @param callback 接收(previousRound, currentRound)的回调函数
     */
    void setRollbackAnimationCallback(const std::function<void(const RoundState*, const RoundState*)>& callback);

    /**
     * @brief 从当前游戏状态初始化第一个回合
     * 应在游戏设置后调用一次
     */
    void initializeFirstRound();

private:
    /**
     * @brief 捕获游戏模型的当前状态
     * @return 新的RoundState实例
     */
    RoundState* captureCurrentState();

    /**
     * @brief 将游戏模型恢复到特定回合状态
     * @param roundState 要恢复的回合状态
     */
    void restoreState(const RoundState* roundState);
};

#endif // __GAME_MEMORY_H__

