#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/stringbuffer.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 步骤1：构建JSON文件的完整路径
    std::string filename = StringUtils::format("levels/level_%d.json", levelId);
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    
    // 步骤2：读取文件内容
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (jsonStr.empty())
    {
        CCLOG("加载关卡配置文件失败: %s", fullPath.c_str());
        return nullptr;
    }
    
    // 步骤3：解析JSON文档
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    
    if (doc.HasParseError())
    {
        CCLOG("JSON解析错误: %d", doc.GetParseError());
        return nullptr;
    }
    
    // 步骤4：实例化LevelConfig对象
    LevelConfig* config = new LevelConfig();
    config->setLevelId(levelId);
    
    // 步骤5：解析游戏区域卡牌
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const rapidjson::Value& playfield = doc["Playfield"];
        std::vector<CardConfig> playfieldCards;
        
        // 遍历游戏区域卡牌数组
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++)
        {
            const rapidjson::Value& cardData = playfield[i];
            CardConfig cardConfig;
            
            // 解析卡牌面值
            if (cardData.HasMember("CardFace"))
            {
                cardConfig.cardFace = parseFaceType(cardData["CardFace"].GetInt());
            }
            
            // 解析卡牌花色
            if (cardData.HasMember("CardSuit"))
            {
                cardConfig.cardSuit = parseSuitType(cardData["CardSuit"].GetInt());
            }
            
            // 解析卡牌位置
            if (cardData.HasMember("Position"))
            {
                const rapidjson::Value& pos = cardData["Position"];
                if (pos.HasMember("x") && pos.HasMember("y"))
                {
                    cardConfig.position.x = pos["x"].GetFloat();
                    cardConfig.position.y = pos["y"].GetFloat();
                }
            }
            
            playfieldCards.push_back(cardConfig);
        }
        
        config->setPlayfieldCards(playfieldCards);
    }
    
    // 步骤6：解析堆叠卡牌
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& stack = doc["Stack"];
        std::vector<CardConfig> stackCards;
        
        // 遍历堆叠卡牌数组
        for (rapidjson::SizeType i = 0; i < stack.Size(); i++)
        {
            const rapidjson::Value& cardData = stack[i];
            CardConfig cardConfig;
            
            // 解析卡牌面值
            if (cardData.HasMember("CardFace"))
            {
                cardConfig.cardFace = parseFaceType(cardData["CardFace"].GetInt());
            }
            
            // 解析卡牌花色
            if (cardData.HasMember("CardSuit"))
            {
                cardConfig.cardSuit = parseSuitType(cardData["CardSuit"].GetInt());
            }
            
            // 解析卡牌位置
            if (cardData.HasMember("Position"))
            {
                const rapidjson::Value& pos = cardData["Position"];
                if (pos.HasMember("x") && pos.HasMember("y"))
                {
                    cardConfig.position.x = pos["x"].GetFloat();
                    cardConfig.position.y = pos["y"].GetFloat();
                }
            }
            
            stackCards.push_back(cardConfig);
        }
        
        config->setStackCards(stackCards);
    }
    
    return config;
}

CardFaceType LevelConfigLoader::parseFaceType(int faceValue)
{
    // JSON使用基于0的索引（0=A, 1=2, 2=3, ..., 12=K）
    // 这与我们的枚举直接匹配
    if (faceValue >= 0 && faceValue < CFT_NUM_CARD_FACE_TYPES)
    {
        return static_cast<CardFaceType>(faceValue);
    }
    return CFT_NONE;
}

CardSuitType LevelConfigLoader::parseSuitType(int suitValue)
{
    // 验证花色值是否在有效范围内
    if (suitValue >= 0 && suitValue < CST_NUM_CARD_SUIT_TYPES)
    {
        return static_cast<CardSuitType>(suitValue);
    }
    return CST_NONE;
}

