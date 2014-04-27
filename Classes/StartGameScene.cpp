#include "StartGameScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* StartGameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StartGameLayer *layer = StartGameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartGameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        return false;
    }
    
    CCSize winSzie = CCDirector::sharedDirector()->getWinSize();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "images/CloseNormal.png",
                                        "images/CloseSelected.png",
                                        this,
                                        menu_selector(StartGameLayer::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(winSzie.width - pCloseItem->getContentSize().width/2 ,
                                  pCloseItem->getContentSize().height/2));

    /////////////////////////////
    // 3. add your codes below...

	//创建Play菜单项 
	CCMenuItemImage *pPlayItem = CCMenuItemImage::create(
		"images/btn-play-normal.png",
		"images/btn-play-selected.png",
		this,
		menu_selector(StartGameLayer::menuPlayCallback));

	// 设置Play菜单项的位置
	pPlayItem->setPosition(ccp(winSzie.width/2, winSzie.height/2.0 + 80));

	//创建High Scores菜单项 
	CCMenuItemImage *pScoreItem = CCMenuItemImage::create(
		"images/btn-highscores-normal.png",
		"images/btn-highscores-selected.png",
		this,
		menu_selector(StartGameLayer::menuHighScoresCallback));

	// 设置High Scores菜单项的位置
	pScoreItem->setPosition(winSzie.width/2, winSzie.height/2);

	//创建About菜单项 
	CCMenuItemImage *pAboutItem = CCMenuItemImage::create(
		"images/btn-about-normal.png",
		"images/btn-about-selected.png",
		this,
		menu_selector(StartGameLayer::menuAboutCallback));

	// 设置About菜单项的位置
	pAboutItem->setPosition(winSzie.width/2, winSzie.height/2 - 80);

	// 创建主菜单，将Play、High scores、About菜单项添加至主菜单中
	CCMenu* pMainMenu = CCMenu::create(pPlayItem, pScoreItem, pAboutItem, pCloseItem, NULL);
	pMainMenu->setPosition(CCPointZero);
	this->addChild(pMainMenu, 1);
    
    return true;
}


void StartGameLayer::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

//	Play回调函数
void StartGameLayer::menuPlayCallback(CCObject* pSender)
{
	//CCMessageBox("You pressed the start game button.","Alert");
	CCLog("Start the SpaceWar game!");

	//实例化一个场景GameScene
	CCScene *scene = GameLayer::scene();

	//进入游戏场景，并设置过渡效果(此处为淡入淡出)
	//CCDirector::sharedDirector()->replaceScene(pScene);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, scene));
}

// High Scores回调函数
void  StartGameLayer::menuHighScoresCallback(CCObject* pSender)
{
	CCMessageBox("Your highest score is: 4500", "Highest Score");
}

//	About回调函数
void  StartGameLayer::menuAboutCallback(CCObject* pSender)
{
	CCMessageBox("这是一个SpaceWar游戏项目，素材来源于网络，使用Cocos2d-x 2.2.3进行移植！","About");
}