#ifndef _GAME_OVER_SCENE_H
#define _GAME_OVER_SCENE_H

#include "cocos2d.h"

class GameOverLayer : 
	public cocos2d::CCLayerColor
{
public:
	GameOverLayer(void);
	~GameOverLayer(void);
	bool initWithWon(bool won);

	static cocos2d::CCScene* sceneWithWon(bool won);
	static GameOverLayer* createWithWon(bool won);
	void gameOverDone();
};

#endif	//_GAME_OVER_SCENE_H


