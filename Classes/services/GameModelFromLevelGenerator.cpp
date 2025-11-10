#include "GameModelFromLevelGenerator.h"

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
    if (!levelConfig)
    {
        return nullptr;
    }
    
    // 步骤1：创建游戏模型实例
    GameModel* gameModel = new GameModel();
    
    // 步骤2：创建游戏区域卡牌
    GameModelFromLevelGenerator::_createPlayfieldCards(levelConfig, gameModel);
    
    // 步骤3：创建堆叠卡牌
    GameModelFromLevelGenerator::_createStackCards(levelConfig, gameModel);
    
    // 步骤4：通过从堆叠弹出并推入手牌堆叠来选择初始托盘卡牌
    CardModel* topCard = gameModel->popStackCard();
    if (topCard)
    {
        // 将托盘卡牌定位在堆叠区域的右侧
        topCard->setPosition(cocos2d::Vec2(700, 290));
        gameModel->pushHandStackCard(topCard);
    }

    // 步骤5：确保初始可见性状态正确
    gameModel->updatePlayfieldVisibility();
    gameModel->updateStackVisibility();
    
    return gameModel;
}

void GameModelFromLevelGenerator::_createPlayfieldCards(const LevelConfig* config, GameModel* gameModel)
{
    // 获取游戏区域卡牌配置
    const std::vector<CardConfig>& cardConfigs = config->getPlayfieldCards();
    
    // 遍历配置，为每张卡牌创建模型
    for (const auto& cardConfig : cardConfigs)
    {
        // 获取下一个可用的卡牌ID
        int cardId = gameModel->getNextCardId();
        // 创建卡牌模型
        CardModel* card = new CardModel(cardId, cardConfig.cardFace, cardConfig.cardSuit);
        // 设置卡牌位置
        card->setPosition(cardConfig.position);
        
        // 将卡牌添加到游戏区域
        gameModel->addPlayfieldCard(card);
    }
}

void GameModelFromLevelGenerator::_createStackCards(const LevelConfig* config, GameModel* gameModel)
{
    // 获取堆叠卡牌配置
    const std::vector<CardConfig>& cardConfigs = config->getStackCards();
    
    // 堆叠卡牌以相反顺序推送以保持堆叠语义
    // 这样最后一张卡牌会在堆叠顶部
    for (int i = cardConfigs.size() - 1; i >= 0; i--)
    {
        const CardConfig& cardConfig = cardConfigs[i];
        // 获取下一个可用的卡牌ID
        int cardId = gameModel->getNextCardId();
        // 创建卡牌模型
        CardModel* card = new CardModel(cardId, cardConfig.cardFace, cardConfig.cardSuit);
        
        // 堆叠卡牌从堆叠区域的左侧附近开始
        card->setPosition(cocos2d::Vec2(200, 290));
        
        // 将卡牌添加到堆叠
        gameModel->addStackCard(card);
    }
}

