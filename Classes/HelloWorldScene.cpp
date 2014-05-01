#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "Monster.h"
#include "LevelManager.h"

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
		CC_BREAK_IF(! CCLayerColor::initWithColor(LevelManager::sharedInstance()->curLevel()->getBackgroundColor()));

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		_player = CCSprite::create("images/player2.png");
		//	�������ǵ�λ��
		_player->setPosition(ccp(_player->getContentSize().width/2, winSize.height/2));
		this->addChild(_player);

		this->_monsters = CCArray::create();
		this->_monsters->retain();
		this->_projectiles = CCArray::create();
		this->_projectiles->retain();


		//������ʾ��ǩ
		scoreLabel = CCLabelTTF::create("Score: 0", "Arial", 30);
		//���ñ�ǩ������ɫΪ��ɫ
		scoreLabel->setColor(ccc3(255,120,0));
		scoreLabel->setPosition(ccp(100, winSize.height - 30));
		this->addChild(scoreLabel, 1);

		// �˳��˵���
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"images/CloseNormal.png",
			"images/CloseSelected.png",
			this,
			menu_selector(GameLayer::gameOverCallback));

		// �����˳��˵����λ��Ϊ���½�
		pCloseItem->setPosition(ccp(winSize.width - pCloseItem->getContentSize().width/2 ,
			pCloseItem->getContentSize().height/2));

		// create menu, it's an autorelease object
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu, 1);

		this->schedule(schedule_selector(GameLayer::gameLogic), LevelManager::sharedInstance()->curLevel()->getSecsPerSpawn());

		//	���������¼�
		this->setTouchEnabled(true);

		//  ������ײ���
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
	_nextProjectile = NULL;
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
	//CCSprite* monster = CCSprite::create("images/monster.png");
	Monster	*monster = NULL;
	//	This will give a 50% chance to spawn each type of monster. 
	if (0 == (rand() % 2))
	{
		monster = WeakAndFastMonster::create();
	} 
	else
	{
		monster = StrongAndSlowMonster::create();
	}

	//	Determine where to spawn the monster along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int minY = monster->getContentSize().height / 2;
	int maxY = winSize.height - monster->getContentSize().height/2;
	int rangeY = maxY - minY;
	// ������ɹ����Y����
	int actualY = (rand() % rangeY) + minY;

	// Create the monster slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated above
	monster->setPosition(ccp(winSize.width + monster->getContentSize().width/2, actualY));
	this->addChild(monster);

	// Determine speed of the monster
	//	Since you��ve moved the speed of the monsters into the classes, modify the min/max duration lines as follows
	int minDuration = monster->getMinMoveDuration();	//2.0
	int maxDuration = monster->getMaxMoveDuration();	//4.0
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

	//	������ǰ�������ӵ�
	CCARRAY_FOREACH(_projectiles, pObject1)
	{
		CCSprite *pProjectile = (CCSprite*)pObject1;

		bool monsterHit = false;

		CCArray *monstersToDelete = CCArray::create();

		//	������ǰ���еĹ���
		CCARRAY_FOREACH(_monsters, pObject2)
		{
			Monster *pMonster = (Monster*)pObject2;
		
			if(pProjectile->boundingBox().intersectsRect(pMonster->boundingBox()))
			{
				//So basically, instead of instantly killing the monster, you subtract an HP and only destroy it if it��s 0 or lower.
				//Also, note that you break out of the loop if the projectile hits a monster, 
				//which means the projectile can only hit one monster per shot.
				monsterHit = true;
				pMonster->setHp(pMonster->getHp() - 1);
				if (pMonster->getHp() <= 0)
				{
					monstersToDelete->addObject(pMonster);
				}

				break;
			}
		}

		//	������Ҫɾ���Ĺ�������
		CCARRAY_FOREACH(monstersToDelete, pObject2)
		{
			Monster *monster = (Monster*)pObject2;
			//	�ӵ�ǰ���������Ƴ����ӵ����еĹ���
			_monsters->removeObject(monster);
			//	�ӳ������Ƴ����ӵ����еĹ���
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

		if(monsterHit)
		{
			projectilesToDelete->addObject(pProjectile);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/explosion.wav");
		}

		monstersToDelete->release();
	}

		//	������ײ�����ӵ�����
		CCARRAY_FOREACH(projectilesToDelete, pObject1)
		{
			CCSprite *pProjectile = (CCSprite *)pObject1;
			//	���������Ƴ��ӵ�
			_projectiles->removeObject(pProjectile);
			//	�ӳ������Ƴ��ӵ�
			this->removeChild(pProjectile, true);
		}

		projectilesToDelete->release();
}

void GameLayer::spriteMoveFinished(CCNode *sender)
{
	CCSprite *sprite = (CCSprite*)sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1)	//���ǹ��ﾫ��
	{
		//	�Ƴ����ﾫ��
		_monsters->removeObject(sprite);

		//�л�����Ϸ��������
		CCScene *gameOverScene = GameOverLayer::sceneWithWon(false);
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}
	else if(sprite->getTag() == 2)	//�����ӵ�����
	{
		//	�Ƴ��ӵ�
		_projectiles->removeObject(sprite);
	}
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (_nextProjectile != NULL)
	{
		return;
	}

	//	Choose one of the touches to work with
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = this->convertTouchToNodeSpace(touch);

	//	Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_nextProjectile = CCSprite::create("images/projectile2.png");
	_nextProjectile->retain();
	_nextProjectile->setPosition(ccp(20, winSize.height/2));

	//	Determine offset of location to projectile
	CCPoint offset = ccpSub(location, _nextProjectile->getPosition());

	//	Bail out if you are shooting down or backwards
	if(offset.x <= 0) 
	{
		return;
	}

	//	Determine where you wish to shoot the projectile to
	int realX = winSize.width + _nextProjectile->getContentSize().width / 2;
	float ratio = (float)offset.y / (float)offset.x;
	int realY = realX * ratio + _nextProjectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

	//	Determine the length of how far you're shooting
	int offRealX = realX - _nextProjectile->getPositionX();
	int offRealY = realY - _nextProjectile->getPositionY();
	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
	float velocity = 480 / 1; //	480pixels/1sec
	float realMoveDuration = length / velocity;

	//	Determine angle to face
	//	the angle you want to rotate is equal to the arctangle of the Y offset divided by the X offset, the result will be in radius.
	float angleRadians = atanf((float)offRealY / (float)offRealX);
	// convert radians to degrees, because Cocos2D deals with degrees.
	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);	
	float cocosAngle = -1 * angleDegrees;	// rotate in  counterclockwise
	//	Would take 0.5 seconds to rotate 180 degrees, or half a cicle
	float rotateDegreesPerSecond = 180 / 0.5;
	float degreesDiff = _player->getRotation() - cocosAngle;
	//	calculate how long this particular rotation should take
	float rotateDuration = fabs(degreesDiff / rotateDegreesPerSecond);
	_player->runAction(CCSequence::create(CCRotateTo::create(rotateDuration, cocosAngle),
		CCCallFunc::create(this, callfunc_selector(GameLayer::finishShoot)), NULL));
	
	//	Move projectile to actual endpoint
	_nextProjectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), 
		CCCallFuncN::create(this,callfuncN_selector(GameLayer::spriteMoveFinished)), NULL));

	_nextProjectile->setTag(2);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/pew-pew-lei.wav");
}

void GameLayer::finishShoot()
{
	this->addChild(_nextProjectile);
	_projectiles->addObject(_nextProjectile);

	_nextProjectile->release();
	_nextProjectile = NULL;
}

void GameLayer::gameOverCallback(CCObject* pSender)
{
	CCScene *gameOverScene = GameOverLayer::sceneWithWon(false);
	CCDirector::sharedDirector()->replaceScene(gameOverScene);
}


