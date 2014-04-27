#ifndef _GAME_SCENE_H
#define _GAME_SCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class GameLayer : public cocos2d::CCLayerColor
{
public:
	GameLayer();
	~GameLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(GameLayer);

	void addMonster();
	void spriteMoveFinished(CCNode *sender);
	void gameLogic(float dt);
	void update(float dt);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void finishShoot();
	void gameOverCallback(CCObject* pSender);

private:
	cocos2d::CCLabelTTF* scoreLabel;	//·ÖÊý±êÇ©

	cocos2d::CCArray	*_monsters;
	cocos2d::CCArray	*_projectiles;

	int	_monstersDestroyed;	//	the projectiles the player has destroyed

	cocos2d::CCSprite	*_player;	//	the player role sprite
	cocos2d::CCSprite	*_nextProjectile;
};

#endif	//_GAME_SCENE_H
