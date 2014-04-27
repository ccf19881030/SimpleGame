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

	//����Play�˵��� 
	CCMenuItemImage *pPlayItem = CCMenuItemImage::create(
		"images/btn-play-normal.png",
		"images/btn-play-selected.png",
		this,
		menu_selector(StartGameLayer::menuPlayCallback));

	// ����Play�˵����λ��
	pPlayItem->setPosition(ccp(winSzie.width/2, winSzie.height/2.0 + 80));

	//����High Scores�˵��� 
	CCMenuItemImage *pScoreItem = CCMenuItemImage::create(
		"images/btn-highscores-normal.png",
		"images/btn-highscores-selected.png",
		this,
		menu_selector(StartGameLayer::menuHighScoresCallback));

	// ����High Scores�˵����λ��
	pScoreItem->setPosition(winSzie.width/2, winSzie.height/2);

	//����About�˵��� 
	CCMenuItemImage *pAboutItem = CCMenuItemImage::create(
		"images/btn-about-normal.png",
		"images/btn-about-selected.png",
		this,
		menu_selector(StartGameLayer::menuAboutCallback));

	// ����About�˵����λ��
	pAboutItem->setPosition(winSzie.width/2, winSzie.height/2 - 80);

	// �������˵�����Play��High scores��About�˵�����������˵���
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

//	Play�ص�����
void StartGameLayer::menuPlayCallback(CCObject* pSender)
{
	//CCMessageBox("You pressed the start game button.","Alert");
	CCLog("Start the SpaceWar game!");

	//ʵ����һ������GameScene
	CCScene *scene = GameLayer::scene();

	//������Ϸ�����������ù���Ч��(�˴�Ϊ���뵭��)
	//CCDirector::sharedDirector()->replaceScene(pScene);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, scene));
}

// High Scores�ص�����
void  StartGameLayer::menuHighScoresCallback(CCObject* pSender)
{
	CCMessageBox("Your highest score is: 4500", "Highest Score");
}

//	About�ص�����
void  StartGameLayer::menuAboutCallback(CCObject* pSender)
{
	CCMessageBox("����һ��SpaceWar��Ϸ��Ŀ���ز���Դ�����磬ʹ��Cocos2d-x 2.2.3������ֲ��","About");
}