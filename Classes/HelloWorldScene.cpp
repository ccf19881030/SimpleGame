#include "HelloWorldScene.h"
#include "GameOverScene.h"

USING_NS_CC;

CCScene* GameLayer::scene()
{
	CCScene *scene = NULL;
	do 
	{
		//	'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		//	'layer' is an autorelease object
		GameLayer *layer = GameLayer::create();
		CC_BREAK_IF(! layer);

		//	add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	//	return the scene
	return scene;
}

bool GameLayer::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255,255,255,255)));

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		_player = CCSprite::create("images/player2.png");
		//	设置主角的位置
		_player->setPosition(ccp(_player->getContentSize().width/2, winSize.height/2));
		this->addChild(_player);

		this->_monsters = CCArray::create();
		this->_monsters->retain();
		this->_projectiles = CCArray::create();
		this->_projectiles->retain();


		//分数显示标签
		scoreLabel = CCLabelTTF::create("Score: 0", "Arial", 30);
		//设置标签字体颜色为红色
		scoreLabel->setColor(ccc3(255,120,0));
		scoreLabel->setPosition(ccp(100, winSize.height - 30));
		this->addChild(scoreLabel, 1);

		// 退出菜单项
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"images/CloseNormal.png",
			"images/CloseSelected.png",
			this,
			menu_selector(GameLayer::gameOverCallback));

		// 设置退出菜单项的位置为右下角
		pCloseItem->setPosition(ccp(winSize.width - pCloseItem->getContentSize().width/2 ,
			pCloseItem->getContentSize().height/2));

		// create menu, it's an autorelease object
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu, 1);

		this->schedule(schedule_selector(GameLayer::gameLogic), 1.0);

		//	开启触摸事件
		this->setTouchEnabled(true);

		//  处理碰撞检测
		this->schedule(schedule_selector(GameLayer::update));

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/background-music-aac.wav");

		bRet = true;

	} while (0);


	return bRet;
}


GameLayer::GameLayer()
{
	_monsters = NULL;
	_projectiles = NULL;
	_monstersDestroyed = 0;
	_player = NULL;
}

GameLayer::~GameLayer()
{
	if(_monsters)
	{
		_monsters->release();
		_monsters = NULL;
	}
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
}

void GameLayer::addMonster()
{
	CCSprite* monster = CCSprite::create("images/monster.png");
	//	Determine where to spawn the monster along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int minY = monster->getContentSize().height / 2;
	int maxY = winSize.height - monster->getContentSize().height/2;
	int rangeY = maxY - minY;
	// 随机生成怪物的Y坐标
	int actualY = (rand() % rangeY) + minY;

	// Create the monster slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated above
	monster->setPosition(ccp(winSize.width + monster->getContentSize().width/2, actualY));
	this->addChild(monster);

	// Determine speed of the monster
	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	//	create the actions
	CCMoveTo *actionMove = CCMoveTo::create(actualDuration, ccp(-monster->getContentSize().width/2, actualY));
	CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameLayer::spriteMoveFinished));
	monster->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));

	monster->setTag(1);
	_monsters->addObject(monster);

}

void GameLayer::gameLogic(float dt)
{
	this->addMonster();
}

void GameLayer::update(float dt)
{
	CCArray *projectilesToDelete = CCArray::create();

	CCObject *pObject1 = NULL;
	CCObject *pObject2 = NULL;

	//	遍历当前的所有子弹
	CCARRAY_FOREACH(_projectiles, pObject1)
	{
		CCSprite *pProjectile = (CCSprite*)pObject1;

		CCArray *monstersToDelete = CCArray::create();

		//	遍历当前所有的怪物
		CCARRAY_FOREACH(_monsters, pObject2)
		{
			CCSprite *pMonster = (CCSprite*)pObject2;
			//	判断当前的取得的子弹是否和怪物碰撞，进行碰撞检测
			if(pProjectile->boundingBox().intersectsRect(pMonster->boundingBox()))
			{
				//	将被子弹击中的怪物加入到需要删除的怪物数组
				monstersToDelete->addObject(pMonster);

				break;
			}
		}

		//	遍历需要删除的怪物数组
		CCARRAY_FOREACH(monstersToDelete, pObject2)
		{
			CCSprite *monster = (CCSprite*)pObject2;
			//	从当前的数组中移除被子弹击中的怪物
			_monsters->removeObject(monster);
			//	从场景中移除被子弹击中的怪物
			this->removeChild(monster, true);

			_monstersDestroyed++;
			//	update the scores according the monster number been destroyed
			CCString *scoreMsg = CCString::createWithFormat("Score: %d",_monstersDestroyed * 50);
			scoreLabel->setString(scoreMsg->getCString());

			if (_monstersDestroyed > 30)
			{
				CCScene *gameOverScene = GameOverLayer::sceneWithWon(true);
				CCDirector::sharedDirector()->replaceScene(gameOverScene);
			}
		}

		if(monstersToDelete->count() > 0)
		{
			projectilesToDelete->addObject(pProjectile);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/explosion.wav");
		}

		monstersToDelete->release();

		//	遍历碰撞到的子弹数组
		CCARRAY_FOREACH(projectilesToDelete, pObject1)
		{
			CCSprite *pProjectile = (CCSprite *)pObject1;
			//	从数组中移除子弹
			_projectiles->removeObject(pProjectile);
			//	从场景中移除子弹
			this->removeChild(pProjectile, true);
		}

		projectilesToDelete->release();
	}
}

void GameLayer::spriteMoveFinished(CCNode *sender)
{
	CCSprite *sprite = (CCSprite*)sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1)	//若是怪物精灵
	{
		//	移除怪物精灵
		_monsters->removeObject(sprite);

		//切换至游戏结束场景
		CCScene *gameOverScene = GameOverLayer::sceneWithWon(false);
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}
	else if(sprite->getTag() == 2)	//若是子弹精灵y
	{
		//	移除子弹
		_projectiles->removeObject(sprite);
	}
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	//	Choose one of the touches to work with
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = this->convertTouchToNodeSpace(touch);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("images/projectile2.png");
	projectile->setPosition(ccp(20, winSize.height/2));

	//	Determine offset of location to projectile
	CCPoint offset = ccpSub(location, projectile->getPosition());

	//	Bail out if you are shooting down or backwards
	if(offset.x <= 0) return;

	//	ok to add now - we've double checked position
	//	添加子弹
	this->addChild(projectile);

	int realX = winSize.width + projectile->getContentSize().width/2;
	float ratio = (float)offset.y / (float)offset.x;
	int realY = (realX * ratio) + projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

	//	Determine the length of how far you're shooting
	int offRealX = realX - projectile->getPositionX();
	int offRealY = realY - projectile->getPositionY();
	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
	float velocity = 480/1; //	480pixels/1sec
	float realMoveDuration = length/velocity;

	//	Move projectile to actual endpoint
	CCMoveTo *actionMove = CCMoveTo::create(realMoveDuration, realDest);
	CCCallFuncN *actionMoveDone = CCCallFuncN::create(this,callfuncN_selector(GameLayer::spriteMoveFinished));
	projectile->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));

	projectile->setTag(2);
	_projectiles->addObject(projectile);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/pew-pew-lei.wav");
}

void GameLayer::gameOverCallback(CCObject* pSender)
{
	CCScene *gameOverScene = GameOverLayer::sceneWithWon(false);
	CCDirector::sharedDirector()->replaceScene(gameOverScene);
}


