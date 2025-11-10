#include "CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

CardView* CardView::create(const CardModel* model)
{
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(model))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool CardView::init(const CardModel* model)
{
    if (!Node::init())
    {
        return false;
    }
    
    _cardModel = model;
    _touchListener = nullptr;
    
    // 创建背景精灵
    std::string bgPath = CardResConfig::getCardBackgroundPath();
    _backgroundSprite = Sprite::create(bgPath);
    if (_backgroundSprite)
    {
        this->addChild(_backgroundSprite, 0);
        
        // 根据背景纹理设置卡牌大小
        Size cardSize = _backgroundSprite->getContentSize();
        this->setContentSize(cardSize);
        _backgroundSprite->setPosition(cardSize.width / 2, cardSize.height / 2);
    }
    
    // 创建背面精灵
    std::string backPath = CardResConfig::getCardBackPath();
    _backSprite = Sprite::create(backPath);
    if (_backSprite)
    {
        this->addChild(_backSprite, 1);
        Size cardSize = this->getContentSize();
        if (cardSize.width > 0 && cardSize.height > 0)
        {
            _backSprite->setPosition(cardSize.width / 2, cardSize.height / 2);
        }
        else
        {
            Size backSize = _backSprite->getContentSize();
            this->setContentSize(backSize);
            _backSprite->setPosition(backSize.width / 2, backSize.height / 2);
        }
    }
    
    // 创建花色图标精灵
    std::string suitPath = CardResConfig::getSuitIconPath(model->getSuitType());
    _suitSprite = Sprite::create(suitPath);
    if (_suitSprite)
    {
        this->addChild(_suitSprite, 2);
        // 定位在左上角附近
        Size cardSize = this->getContentSize();
        _suitSprite->setPosition(cardSize.width * 0.2f, cardSize.height * 0.8f);
        _suitSprite->setScale(0.3f);
    }
    
    // 创建大号中心数字精灵
    std::string bigNumPath = CardResConfig::getBigNumberPath(model->getFaceType(), model->getSuitType());
    _bigNumberSprite = Sprite::create(bigNumPath);
    if (_bigNumberSprite)
    {
        this->addChild(_bigNumberSprite, 2);
        // 定位在中心
        Size cardSize = this->getContentSize();
        _bigNumberSprite->setPosition(cardSize.width / 2, cardSize.height / 2);
        _bigNumberSprite->setScale(0.8f);
    }
    
    // 创建小号角落数字精灵
    std::string smallNumPath = CardResConfig::getSmallNumberPath(model->getFaceType(), model->getSuitType());
    _smallNumberSprite = Sprite::create(smallNumPath);
    if (_smallNumberSprite)
    {
        this->addChild(_smallNumberSprite, 2);
        // 定位在左上角附近
        Size cardSize = this->getContentSize();
        _smallNumberSprite->setPosition(cardSize.width * 0.2f, cardSize.height * 0.9f);
        _smallNumberSprite->setScale(0.4f);
    }
    
    // 应用模型中的初始位置
    this->setPosition(model->getPosition());
    
    // 初始化触摸处理
    _initTouchEvent();
    
    // 与模型状态同步可见性
    updateDisplay();
    
    return true;
}

CardView::~CardView()
{
    if (_touchListener)
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
}

void CardView::setClickCallback(const std::function<void(int)>& callback)
{
    _clickCallback = callback;
}

void CardView::playMoveAnimation(const cocos2d::Vec2& targetPos, const std::function<void()>& callback)
{
    // 使用简单的MoveTo动作播放移动动画
    float duration = 0.3f;
    auto moveTo = MoveTo::create(duration, targetPos);
    
    if (callback)
    {
        auto callbackAction = CallFunc::create(callback);
        auto sequence = Sequence::create(moveTo, callbackAction, nullptr);
        this->runAction(sequence);
    }
    else
    {
        this->runAction(moveTo);
    }
}

void CardView::playFlipAnimation(const std::function<void()>& callback)
{
    // 使用缩放效果播放简单的翻转动画
    float duration = 0.2f;
    auto scaleX1 = ScaleTo::create(duration / 2, 0.0f, 1.0f);
    auto scaleX2 = ScaleTo::create(duration / 2, 1.0f, 1.0f);
    
    if (callback)
    {
        auto callbackAction = CallFunc::create(callback);
        auto sequence = Sequence::create(scaleX1, scaleX2, callbackAction, nullptr);
        this->runAction(sequence);
    }
    else
    {
        auto sequence = Sequence::create(scaleX1, scaleX2, nullptr);
        this->runAction(sequence);
    }
}

void CardView::updateDisplay()
{
    if (!_cardModel)
    {
        return;
    }
    
    // 根据模型状态更新位置
    this->setPosition(_cardModel->getPosition());
    
    bool isFaceUp = _cardModel->isFlipped();
    
    // 辅助函数：将CardFaceType转换为显示字符串
    auto faceToString = [](CardFaceType face) -> std::string {
        switch (face) {
            case CFT_ACE: return "A";
            case CFT_TWO: return "2";
            case CFT_THREE: return "3";
            case CFT_FOUR: return "4";
            case CFT_FIVE: return "5";
            case CFT_SIX: return "6";
            case CFT_SEVEN: return "7";
            case CFT_EIGHT: return "8";
            case CFT_NINE: return "9";
            case CFT_TEN: return "10";
            case CFT_JACK: return "J";
            case CFT_QUEEN: return "Q";
            case CFT_KING: return "K";
            default: return "?";
        }
    };
    
    // 辅助函数：将CardSuitType转换为显示字符串
    auto suitToString = [](CardSuitType suit) -> std::string {
        switch (suit) {
            case CST_CLUBS: 
                return "C";
            case CST_DIAMONDS: 
                return "D";
            case CST_HEARTS: 
                return "H";
            case CST_SPADES: 
                return "S";
            default: 
                return "?";
        }
    };
    
    CCLOG("CardView::updateDisplay - 卡牌 %d (%s%s): %s (位置: %.0f, %.0f)", 
          _cardModel->getCardId(),
          suitToString(_cardModel->getSuitType()).c_str(),
          faceToString(_cardModel->getFaceType()).c_str(),
          isFaceUp ? "正面" : "背面",
          _cardModel->getPosition().x, 
          _cardModel->getPosition().y);
    
    // 根据卡牌是否正面朝上设置各个精灵的可见性
    if (_backgroundSprite)
    {
        _backgroundSprite->setVisible(isFaceUp);
    }
    if (_suitSprite)
    {
        _suitSprite->setVisible(isFaceUp);
    }
    if (_bigNumberSprite)
    {
        _bigNumberSprite->setVisible(isFaceUp);
    }
    if (_smallNumberSprite)
    {
        _smallNumberSprite->setVisible(isFaceUp);
    }
    if (_backSprite)
    {
        _backSprite->setVisible(!isFaceUp);
    }
}

const CardModel* CardView::getCardModel() const
{
    return _cardModel;
}

void CardView::_initTouchEvent()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    
    _touchListener->onTouchBegan = CC_CALLBACK_2(CardView::_onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

bool CardView::_onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // 只有正面朝上的卡牌才能响应触摸
    if (!_cardModel || !_cardModel->isFlipped())
    {
        return false;
    }
    
    // 获取世界空间中的触摸位置
    Vec2 touchLocation = touch->getLocation();
    
    // 转换为本地节点空间
    Vec2 localLocation = this->convertToNodeSpace(touchLocation);
    
    // 检查触摸是否在卡牌边界内
    Rect rect = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    if (rect.containsPoint(localLocation))
    {
        // 触发点击回调函数
        if (_clickCallback && _cardModel)
        {
            _clickCallback(_cardModel->getCardId());
        }
        return true;
    }
    
    return false;
}

