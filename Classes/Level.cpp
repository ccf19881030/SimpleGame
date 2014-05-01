#include "Level.h"
using namespace cocos2d;

bool Level::initWithLevelNum(int levelNum, float secsPerSpawn, ccColor4B backgroundColor)
{
	bool bRet = false;

	do 
	{
		this->_levelNum = levelNum;
		this->_secsPerSpawn = secsPerSpawn;
		this->_backgroundColor = backgroundColor;
	} while (0);

	return bRet;
}