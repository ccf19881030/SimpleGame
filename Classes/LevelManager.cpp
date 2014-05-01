#include "LevelManager.h"
using namespace cocos2d;

LevelManager::LevelManager(void)
{
	_levels = NULL;
}

LevelManager::~LevelManager(void)
{
	if (_levels)
	{
		_levels->release();
		_levels = NULL;
	}
}

LevelManager* LevelManager::sharedInstance(void)
{
	static LevelManager * s_SharedLevelManager = NULL;
	if (!s_SharedLevelManager)
	{
		s_SharedLevelManager = new LevelManager();
		s_SharedLevelManager->init();
	}

	return s_SharedLevelManager;
}

Level * LevelManager::curLevel(void)
{
	if (_curLevelIdx >= (int)_levels->count())
	{
		return NULL;
	}

	return (Level*)_levels->objectAtIndex(_curLevelIdx);
}

void LevelManager::nextLevel(void)
{
	_curLevelIdx++;
}

void LevelManager::reset(void)
{
	_curLevelIdx = 0;
}

bool LevelManager::init()
{
	_curLevelIdx = 0;
	Level *level1 = new Level();
	level1->initWithLevelNum(1, 2, ccc4(255, 255, 255, 255));
	level1->autorelease();
	Level *level2 = new Level();
	level2->initWithLevelNum(2, 1, ccc4(100, 150, 20, 255));
	level2->autorelease();
	_levels = CCArray::create(level1, level2, NULL);
	_levels->retain();

	return true;
}

void LevelManager::end()
{
	this->release();
}