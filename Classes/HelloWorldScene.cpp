/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "controllers/GameController.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 步骤1：直接创建并启动GameController
    GameController* gameController = GameController::create();
    if (gameController)
    {
        // 步骤2：加载关卡1
        gameController->startGame(1);
        return gameController;
    }
    
    // 步骤3：如果创建失败，回退到默认场景
    return HelloWorld::create();
}

// 当文件不存在时打印有用的错误消息，而不是段错误
static void problemLoading(const char* filename)
{
    printf("加载时出错: %s\n", filename);
    printf("根据编译方式，您可能需要在HelloWorldScene.cpp中的文件名前添加'Resources/'\n");
}

// 在"init"时需要初始化实例
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    // 此场景仅作为占位符。GameController处理游戏玩法
    auto label = Label::createWithSystemFont("卡牌游戏加载中...", "Arial", 32);
    if (label)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        label->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
        this->addChild(label);
    }
    
    return true;
}
