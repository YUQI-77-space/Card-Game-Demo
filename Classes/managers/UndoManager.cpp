#include "UndoManager.h"

USING_NS_CC;

UndoManager::UndoManager(GameModel* gameModel)
    : _gameModel(gameModel)
{
}

UndoManager::~UndoManager()
{
    clear();
}

void UndoManager::pushUndoRecord(UndoModel* undoData)
{
    // 注意：UndoManager已废弃，现在使用GameMemory处理状态
    if (undoData)
    {
        _undoStack.push(undoData);
        CCLOG("已推送撤销记录: 源卡牌=%d, 目标卡牌=%d", 
              undoData->getSourceCardId(), undoData->getTargetCardId());
    }
}

bool UndoManager::performUndo()
{
    // 注意：UndoManager已废弃，现在使用GameMemory处理状态
    if (!canUndo())
    {
        CCLOG("无法撤销: 堆叠为空");
        return false;
    }
    
    // 步骤1：检索顶部撤销记录
    UndoModel* undoData = _undoStack.top();
    _undoStack.pop();
    
    if (!undoData || !_gameModel)
    {
        return false;
    }
    
    CCLOG("执行撤销: 源卡牌=%d, 目标卡牌=%d, 类型=%d", 
          undoData->getSourceCardId(), undoData->getTargetCardId(), 
          undoData->getOperationType());
    
    // 步骤2：根据操作类型执行逻辑
    switch (undoData->getOperationType())
    {
        case UOT_STACK_REPLACE:
        {
            // 撤销堆叠替换操作
            // 1. 将当前托盘卡牌（之前的源卡牌）移回堆叠
            // 2. 恢复之前的托盘卡牌（目标卡牌）作为托盘顶部
            
            CardModel* sourceCard = _gameModel->getCardById(undoData->getSourceCardId());
            CardModel* targetCard = _gameModel->getCardById(undoData->getTargetCardId());
            
            if (sourceCard && targetCard)
            {
                // 恢复位置
                sourceCard->setPosition(undoData->getFromPosition());
                targetCard->setPosition(undoData->getToPosition());
                
                // 注意：UndoManager已废弃 - GameMemory现在处理状态
                // 恢复模型状态应该是：将targetCard推回手牌堆叠
                _gameModel->addStackCard(sourceCard);
                
                // 触发撤销动画回调
                if (_undoAnimationCallback)
                {
                    _undoAnimationCallback(undoData);
                }
            }
            break;
        }
        
        case UOT_PLAYFIELD_MATCH:
        {
            // 撤销游戏区域匹配操作
            // 1. 将当前托盘卡牌（之前的源卡牌）移回游戏区域
            // 2. 恢复之前的托盘卡牌（目标卡牌）作为托盘顶部
            
            CardModel* sourceCard = _gameModel->getCardById(undoData->getSourceCardId());
            CardModel* targetCard = _gameModel->getCardById(undoData->getTargetCardId());
            
            if (sourceCard && targetCard)
            {
                // 恢复位置
                sourceCard->setPosition(undoData->getFromPosition());
                targetCard->setPosition(undoData->getToPosition());
                
                // 注意：UndoManager已废弃 - GameMemory现在处理状态
                // 恢复模型状态应该是：将targetCard推回手牌堆叠
                _gameModel->addPlayfieldCard(sourceCard);
                
                // 触发撤销动画回调
                if (_undoAnimationCallback)
                {
                    _undoAnimationCallback(undoData);
                }
            }
            break;
        }
    }
    
    delete undoData;
    return true;
}

bool UndoManager::canUndo() const
{
    // 注意：UndoManager已废弃，现在使用GameMemory处理状态
    return !_undoStack.empty();
}

void UndoManager::clear()
{
    // 注意：UndoManager已废弃，现在使用GameMemory处理状态
    // 清除所有撤销记录
    while (!_undoStack.empty())
    {
        UndoModel* undoData = _undoStack.top();
        _undoStack.pop();
        delete undoData;
    }
}

void UndoManager::setUndoAnimationCallback(const std::function<void(const UndoModel*)>& callback)
{
    _undoAnimationCallback = callback;
}

