#ifndef __PLAYFIELD_CONTROLLER_H__
#define __PLAYFIELD_CONTROLLER_H__

#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include "managers/GameMemory.h"

class StackController;

/**
 * @class PlayFieldController
 * @brief 游戏区域控制器，负责管理游戏区域的卡牌交互
 *
 * 职责说明：
 * - 管理游戏区域卡牌交互
 * - 处理游戏区域卡牌点击事件
 * - 执行匹配和替换逻辑
 */
class PlayFieldController
{
private:
    GameModel* _gameModel;              // 游戏数据模型的引用
    PlayFieldView* _playfieldView;      // 游戏区域的视图
    GameMemory* _gameMemory;            // 游戏内存管理器的引用
    StackController* _stackController;  // 堆叠控制器的引用（用于托盘更新）

public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏数据模型
     * @param gameMemory 游戏内存管理器
     */
    PlayFieldController(GameModel* gameModel, GameMemory* gameMemory);

    /**
     * @brief 析构函数
     */
    ~PlayFieldController();

    /**
     * @brief 初始化内部状态
     * @return 成功返回true，否则返回false
     */
    bool init();

    /**
     * @brief 初始化游戏区域视图并附加到父节点
     * @param parent 父节点（GameView的游戏区域容器）
     */
    void initView(cocos2d::Node* parent);

    /**
     * @brief 处理来自游戏区域的卡牌点击事件
     * @param cardId 被点击卡牌的标识符
     */
    void handleCardClick(int cardId);

    /**
     * @brief 检查卡牌是否可以与当前托盘卡牌匹配
     * @param cardId 被点击卡牌的标识符
     * @return 可以匹配返回true，否则返回false
     */
    bool checkCanMatch(int cardId);

    /**
     * @brief 当卡牌匹配时，用游戏区域卡牌替换托盘卡牌
     * @param cardId 匹配卡牌的标识符
     */
    void replaceTrayWithPlayFieldCard(int cardId);

    /**
     * @brief 获取游戏区域视图
     * @return 游戏区域视图的指针
     */
    PlayFieldView* getPlayFieldView() const;

    /**
     * @brief 设置堆叠控制器引用以协调托盘更新
     * @param stackController 堆叠控制器实例
     */
    void setStackController(StackController* stackController);

private:
    /**
     * @brief 检查并处理胜利条件（游戏区域清空）
     */
    void _checkGameVictory();
    
    /**
     * @brief 在屏幕上显示胜利消息
     */
    void _showVictoryMessage();
};

#endif // __PLAYFIELD_CONTROLLER_H__

