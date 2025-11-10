#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"

/**
 * @enum UndoOperationType
 * @brief 可撤销操作的类型
 * 
 * @deprecated 此枚举已废弃，现在使用GameMemory处理状态
 */
enum UndoOperationType
{
    UOT_STACK_REPLACE,      // 从堆叠区域触发的替换
    UOT_PLAYFIELD_MATCH     // 从游戏区域触发的匹配
};

/**
 * @class UndoModel
 * @brief 表示单个撤销记录的数据结构
 *
 * @deprecated 此类已废弃，现在使用GameMemory和RoundState处理状态
 * 
 * 职责说明：
 * - 捕获恢复一个操作所需的数据
 * - 提供对撤销元数据的只读访问
 */
class UndoModel
{
private:
    UndoOperationType _operationType;   // 操作类型
    int _sourceCardId;                  // 源卡牌标识符
    int _targetCardId;                  // 目标卡牌标识符（之前的托盘卡牌）
    cocos2d::Vec2 _fromPos;             // 起始位置
    cocos2d::Vec2 _toPos;               // 目标位置

public:
    /**
     * @brief 构造撤销记录
     * @param opType 操作类型
     * @param sourceId 源卡牌标识符
     * @param targetId 目标卡牌标识符
     * @param from 起始位置
     * @param to 目标位置
     */
    UndoModel(UndoOperationType opType, int sourceId, int targetId,
              const cocos2d::Vec2& from, const cocos2d::Vec2& to);

    /**
     * @brief 析构函数
     */
    ~UndoModel();

    /**
     * @brief 获取操作类型
     * @return 操作类型
     */
    UndoOperationType getOperationType() const;

    /**
     * @brief 获取源卡牌标识符
     * @return 源卡牌标识符
     */
    int getSourceCardId() const;

    /**
     * @brief 获取目标卡牌标识符
     * @return 目标卡牌标识符
     */
    int getTargetCardId() const;

    /**
     * @brief 获取原始位置
     * @return 起始位置
     */
    cocos2d::Vec2 getFromPosition() const;

    /**
     * @brief 获取目标位置
     * @return 目标位置
     */
    cocos2d::Vec2 getToPosition() const;
};

#endif // __UNDO_MODEL_H__

