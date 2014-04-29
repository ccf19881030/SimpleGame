#include "Monster.h"
//	implementation of Monster
bool Monster::initWithFile(const char *pszFilename, int hp, int minMoveDuration, int maxMoveDuration)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCSprite::initWithFile(pszFilename));

		this->_hp = hp;
		this->_minMoveDuration = minMoveDuration;
		this->_maxMoveDuration = maxMoveDuration;

		bRet = true;
	} while (0);

	return bRet;
}

//	implementation of WeakAndFastMonster
bool WeakAndFastMonster::init(void)
{
	return Monster::initWithFile("images/monster.png", 1, 3, 5);
}

//	implementation of StrongAndSlowMonster
bool StrongAndSlowMonster::init(void)
{
	return Monster::initWithFile("images/Monster2.png", 3, 6 ,12);
}