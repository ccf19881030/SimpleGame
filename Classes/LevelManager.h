#ifndef _LEVEL_MANAGER_H
#define _LEVEL_MANAGER_H

#include "Level.h"

class LevelManager :
	public cocos2d::CCObject
{
public:
	static LevelManager *sharedInstance(void);
	Level *curLevel(void);
	void nextLevel(void);
	void reset(void);
	bool init();
	void end();

private:
	LevelManager(void);
	~LevelManager(void);

	cocos2d::CCArray *_levels;
	int _curLevelIdx;
};

#endif