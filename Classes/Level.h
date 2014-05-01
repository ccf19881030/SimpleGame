#ifndef _LEVEL_H
#define _LEVEL_H

#include "cocos2d.h"

class Level : 
	public cocos2d::CCObject
{
public:
	virtual bool initWithLevelNum(int levelNum, float secsPerSpawn, cocos2d::ccColor4B backgroundColor);

protected:
	CC_SYNTHESIZE(int, _levelNum, LevelNum);
	CC_SYNTHESIZE(float, _secsPerSpawn, SecsPerSpawn);
	CC_SYNTHESIZE(cocos2d::ccColor4B, _backgroundColor, BackgroundColor);
};

#endif