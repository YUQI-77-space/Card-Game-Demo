#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "models/GameModel.h"
#include "views/StackView.h"
#include "managers/GameMemory.h"

/**
 * @class StackController
 * @brief 堆叠区域控制器，负责管理牌堆和托盘区域
 *
 * 职责说明：
 * - 管理堆叠区域（抽牌堆和托盘）
 * - 处理堆叠卡牌的点击事件
 * - 执行托盘卡牌替换逻辑
 */
class StackController
{
private:
    GameModel* _gameModel;          // 游戏数据模型的引用
    StackView* _stackView;          // 堆叠区域的视图
    GameMemory* _gameMemory;        // 游戏内存管理器的引用

public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏数据模型
     * @param gameMemory 游戏内存管理器
     */
    StackController(GameModel* gameModel, GameMemory* gameMemory);

    /**
     * @brief 析构函数
     */
    ~StackController();

    /**
     * @brief 初始化内部状态
     * @return 成功返回true，否则返回false
     */
    bool init();

    /**
     * @brief 初始化视图并附加到父节点
     * @param parent 父节点（GameView的堆叠容器）
     */
    void initView(cocos2d::Node* parent);

    /**
     * @brief 处理堆叠卡牌的点击事件
     * @param cardId 被点击卡牌的标识符
     */
    void handleStackCardClick(int cardId);

    /**
     * @brief 用点击的卡牌替换当前托盘卡牌
     * @param cardId 被点击卡牌的标识符
     */
    void replaceTopTrayCard(int cardId);

    /**
     * @brief 获取堆叠视图
     * @return 堆叠视图的指针
     */
    StackView* getStackView() const;
};

#endif // __STACK_CONTROLLER_H__

