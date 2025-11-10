#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "StackController.h"
#include "PlayFieldController.h"
#include "managers/GameMemory.h"

namespace cocos2d {
namespace ui {
class Button;
} // namespace ui
} // namespace cocos2d

/**
 * @class GameController
 * @brief 游戏主控制器，负责协调整个游戏流程
 *
 * 职责说明：
 * - 管理整体游戏流程
 * - 协调模型和视图之间的交互
 * - 处理游戏初始化和场景转换
 */
class GameController : public cocos2d::Scene
{
private:
    GameModel* _gameModel;                      // 游戏数据模型
    GameView* _gameView;                        // 根游戏视图
    StackController* _stackController;          // 堆叠区域控制器
    PlayFieldController* _playfieldController;  // 游戏区域控制器
    GameMemory* _gameMemory;                    // 基于回合的游戏内存管理器
    cocos2d::ui::Button* _undoButton;           // 撤销按钮UI元素

public:
    /**
     * @brief 工厂方法创建GameController
     * @return 新创建的GameController实例
     */
    static GameController* create();

    /**
     * @brief 初始化控制器
     * @return 成功返回true，否则返回false
     */
    virtual bool init();

    /**
     * @brief 析构函数
     */
    virtual ~GameController();

    /**
     * @brief 使用指定关卡启动游戏
     * @param levelId 要加载的关卡标识符
     * @return 成功返回true，否则返回false
     */
    bool startGame(int levelId);

private:
    /**
     * @brief 初始化核心视图层次结构
     */
    void _initView();

    /**
     * @brief 创建撤销按钮UI元素
     */
    void _createUndoButton();

    /**
     * @brief 处理撤销按钮点击事件
     */
    void _handleUndoButtonClick();

    /**
     * @brief 处理由GameMemory触发的回滚动画
     * @param previousRound 要恢复的上一个回合状态
     * @param currentRound 当前正在回滚的回合状态
     */
    void _handleRollbackAnimation(const RoundState* previousRound, const RoundState* currentRound);

    /**
     * @brief 通过完全重建所有视图来完成回滚
     */
    void _finalizeRollback();
};

#endif // __GAME_CONTROLLER_H__

