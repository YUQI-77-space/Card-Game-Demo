#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/UndoModel.h"
#include "models/GameModel.h"
#include <stack>
#include <functional>

/**
 * @class UndoManager
 * @brief 负责撤销操作的管理器
 *
 * @deprecated 此类已废弃，现在使用GameMemory处理状态和回滚功能
 * 
 * 职责说明：
 * - 维护撤销堆叠
 * - 记录操作历史
 * - 执行撤销逻辑
 * - 通知控制器播放撤销动画
 */
class UndoManager
{
private:
    std::stack<UndoModel*> _undoStack;                          // 撤销堆叠容器
    GameModel* _gameModel;                                      // 游戏模型的引用
    std::function<void(const UndoModel*)> _undoAnimationCallback;  // 撤销动画的回调函数

public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏模型实例
     */
    UndoManager(GameModel* gameModel);

    /**
     * @brief 析构函数
     */
    ~UndoManager();

    /**
     * @brief 将新的撤销记录推入堆叠
     * @param undoData 撤销数据对象
     */
    void pushUndoRecord(UndoModel* undoData);

    /**
     * @brief 执行撤销操作
     * @return 成功返回true，否则返回false
     */
    bool performUndo();

    /**
     * @brief 判断当前是否可以撤销
     * @return 当堆叠不为空时返回true
     */
    bool canUndo() const;

    /**
     * @brief 清除所有撤销记录
     */
    void clear();

    /**
     * @brief 注册用于播放撤销动画的回调函数
     * @param callback 回调函数
     */
    void setUndoAnimationCallback(const std::function<void(const UndoModel*)>& callback);
};

#endif // __UNDO_MANAGER_H__

