#include "UndoModel.h"

UndoModel::UndoModel(UndoOperationType opType, int sourceId, int targetId, 
                     const cocos2d::Vec2& from, const cocos2d::Vec2& to)
    : _operationType(opType)
    , _sourceCardId(sourceId)
    , _targetCardId(targetId)
    , _fromPos(from)
    , _toPos(to)
{
}

UndoModel::~UndoModel()
{
}

UndoOperationType UndoModel::getOperationType() const
{
    return _operationType;
}

int UndoModel::getSourceCardId() const
{
    return _sourceCardId;
}

int UndoModel::getTargetCardId() const
{
    return _targetCardId;
}

cocos2d::Vec2 UndoModel::getFromPosition() const
{
    return _fromPos;
}

cocos2d::Vec2 UndoModel::getToPosition() const
{
    return _toPos;
}

