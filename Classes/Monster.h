#ifndef _MONSTER_H
#define _MONSTER_H

#include "cocos2d.h"

class Monster : 
	public cocos2d::CCSprite
{
public:
    virtual bool initWithFile(const char *pszFilename, int hp, int minMoveDuration, int maxMoveDuration);
protected:
	CC_SYNTHESIZE(int, _hp, Hp);
	CC_SYNTHESIZE(int , _minMoveDuration, MinMoveDuration);
	CC_SYNTHESIZE(int, _maxMoveDuration, MaxMoveDuration);
};

class WeakAndFastMonster : public Monster
{
public:
	virtual bool init(void);
	CREATE_FUNC(WeakAndFastMonster);
};

class StrongAndSlowMonster : public Monster
{
public:
	virtual bool init(void);
	CREATE_FUNC(StrongAndSlowMonster);
};
#endif
